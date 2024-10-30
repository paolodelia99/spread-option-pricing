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

template<std::floating_point Real>
class MCEngine {
public:
    MCEngine(unsigned int num_sim, unsigned int n_timesteps, size_t n_threads);
    ~MCEngine() = default;
    Real operator()(SpreadOption<Real> &option);

private:
    std::pair<std::vector<Real>, std::vector<Real>> _simulatePaths(SpreadOption<Real>& option);
    Real _computeValue(SpreadOption<Real>& option, std::vector<Real>& final_s1, std::vector<Real>& final_s2);
    std::vector<Real> _generateNormalRandomVec();

    unsigned int n_timesteps_;
    unsigned int num_sim_;
    ThreadPool pool_;
};



#endif //MCENGINE_H
