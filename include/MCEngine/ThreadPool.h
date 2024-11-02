//
// Created by paolodelia on 10/30/24.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

#include <cassert>
#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <functional>

class ThreadPool {
public:

    ThreadPool() = default;

    explicit ThreadPool(size_t n_workers, bool set_thread_affinity = false) {
        for (size_t i = 0; i < n_workers; ++i)
        {
            workers_.emplace_back([this](){
                while(true)
                {
                    std::move_only_function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        cv_.wait(lock, [this](){return stop_ || !tasks_.empty();});

                        if (stop_ && tasks_.empty()) return;

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }

                    task();
                }
            });
        }

        if (set_thread_affinity)
            _set_thread_affinity();
    }

    ThreadPool(const ThreadPool& other_pool) = delete;

    ThreadPool(ThreadPool&& other_pool) noexcept
    :workers_(std::move(other_pool.workers_)), stop_(other_pool.stop_), tasks_(std::move(other_pool.tasks_)) {}

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        for(std::thread &worker: workers_)
            worker.join();
    }

    ThreadPool& operator=(const ThreadPool& other_pool) = delete;

    ThreadPool& operator=(ThreadPool&& other_pool) noexcept
    {
        if (this != &other_pool)
        {
            workers_ = std::move(other_pool.workers_);
            tasks_ = std::move(other_pool.tasks_);
            stop_ = other_pool.stop_;
        }
        return *this;
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        std::packaged_task<return_type()> task(std::forward<F>(f), std::forward<Args>(args)...);

        std::future<return_type> res = task.get_future();
        {
            std::unique_lock<std::mutex> lock(mutex_);

            // don't allow enqueueing after stopping the pool
            if(stop_)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks_.emplace([task = std::move(task)]() mutable { std::move(task)(); });
        }
        cv_.notify_one();

        return res;
    }

private:
    void _set_thread_affinity() {
        cpu_set_t cpu_set_0, cpu_set_1, cpu_set_2, cpu_set_3;
        CPU_ZERO(&cpu_set_0);
        CPU_ZERO(&cpu_set_1);
        CPU_ZERO(&cpu_set_2);
        CPU_ZERO(&cpu_set_3);

        CPU_SET(0, &cpu_set_0);
        CPU_SET(1, &cpu_set_1);
        CPU_SET(2, &cpu_set_2);
        CPU_SET(3, &cpu_set_3);

        int i = 0;

        for (auto& worker : workers_)
        {
            if (i % 4 == 0)
                assert(pthread_setaffinity_np(worker.native_handle(), sizeof(cpu_set_t),
                                                &cpu_set_0) == 0);
            else if (i % 4 == 1)
                assert(pthread_setaffinity_np(worker.native_handle(), sizeof(cpu_set_t),
                                                &cpu_set_1) == 0);
            else if (i % 4 == 2)
                assert(pthread_setaffinity_np(worker.native_handle(), sizeof(cpu_set_t),
                                                &cpu_set_2) == 0);
            else
                assert(pthread_setaffinity_np(worker.native_handle(), sizeof(cpu_set_t),
                                                &cpu_set_3) == 0);
            ++i;
        }
    }

    std::mutex mutex_;
    std::condition_variable cv_;
    std::vector<std::thread> workers_;
    bool stop_ = false;
    std::queue<std::move_only_function<void()>> tasks_;
};

#endif //THREADPOOL_H
