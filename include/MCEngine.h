//
// Created by paolodelia on 7/17/24.
//

#ifndef MCENGINE_H
#define MCENGINE_H
#include <random>

#include "SpreadOption.h"

constexpr double MEAN = 0.0;
constexpr double STD = 1.0;

class MCEngine {
public:
    MCEngine(unsigned int num_sim, unsigned int n_timesteps, unsigned int seed);
    void operator()(SpreadOption &option);

private:
    std::pair<std::vector<double>, std::vector<double>> _simulatePaths(SpreadOption& option);
    double _computeValue(SpreadOption& option, std::vector<double> final_s1, std::vector<double> final_s2);
    std::vector<double> _generateNormalRandomVec();

    unsigned int n_timesteps_;
    unsigned int num_sim_;
    unsigned int seed_;
    std::mt19937 generator_;
    std::normal_distribution<double> normal_distribution_;
};



#endif //MCENGINE_H
