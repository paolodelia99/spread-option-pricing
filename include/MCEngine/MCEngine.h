//
// Created by paolodelia on 7/17/24.
//

#ifndef MCENGINE_H
#define MCENGINE_H
#include <concepts>

#include "SpreadOption.h"
#include "MCEngine/ThreadPool.h"

constexpr double MEAN = 0.0;
constexpr double STD = 1.0;
constexpr int N_THREADS = 4;

class MCEngine {
public:
    MCEngine(unsigned int num_sim, unsigned int n_timesteps, size_t n_threads);
    ~MCEngine() = default;
    MCEngine(MCEngine& other_engine);
    MCEngine(MCEngine&& other_engine) noexcept;
    MCEngine& operator=(MCEngine& other_engine);
    MCEngine& operator=(MCEngine&& other_engine) noexcept;

    template<std::floating_point Real>
    Real calculatePrice(SpreadOption<Real> &option);

private:
    template<std::floating_point Real>
    std::pair<std::vector<Real>, std::vector<Real>> _simulatePaths(SpreadOption<Real>& option);

    template<std::floating_point Real>
    Real _computeValue(SpreadOption<Real>& option, std::vector<Real>& final_s1, std::vector<Real>& final_s2);

    template<std::floating_point Real>
    std::vector<Real> _generateNormalRandomVec();

    unsigned int n_timesteps_;
    unsigned int num_sim_;
};



#endif //MCENGINE_H
