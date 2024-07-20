//
// Created by paolodelia on 7/17/24.
//

#include "MCEngine.h"

std::vector<double> _createCorrelatedVec(std::vector<double> v1, std::vector<double> v2, double rho)
{
    std::vector<double> res(v1.size());

    std::transform(v1.begin(), v1.end(), v1.begin(), [rho](double value) {return rho * value; });
    std::transform(v2.begin(), v2.end(), v2.begin(), [rho](double value) {return sqrt(1 - rho * rho) * value; });

    std::transform(v1.begin(), v1.end(), v2.begin(), res.begin(), std::plus<double>());

    return res;
}

MCEngine::MCEngine(unsigned int num_sim, unsigned int n_timesteps, unsigned int seed)
    :num_sim_(num_sim), n_timesteps_(n_timesteps), seed_(seed)
{
    generator_ = std::mt19937(seed_);
    normal_distribution_ = std::normal_distribution<double>(MEAN, STD);
}

double MCEngine::operator()(SpreadOption& option)
{
    auto final_values = _simulatePaths(option);
    return _computeValue(option, final_values.first, final_values.second);
}

std::pair<std::vector<double>, std::vector<double>> MCEngine::_simulatePaths(SpreadOption& option)
{
    std::vector<double> final_s1(num_sim_), final_s2(num_sim_);

    const double dt = option.getExpiration() / n_timesteps_;
    const double sqrt_dt = sqrt(dt);
    const double r = option.getDiscoutRate();
    const double vol_s1 = option.getVolAsset1();
    const double vol_s2 = option.getVolAsset2();

    for (int i = 0; i < num_sim_; ++i)
    {
        double s1_t = option.getCurrentAsset1Price();
        double s2_t = option.getCurrentAsset2Price();
        std::vector<double> u1 = _generateNormalRandomVec();
        std::vector<double> u2 = _generateNormalRandomVec();

        std::vector<double> w1 = std::vector(u1);
        std::vector<double> w2 = _createCorrelatedVec(u1, u2, option.getCorrelation());

        for (int j = 0; j < n_timesteps_; ++j)
        {
            s1_t = s1_t * exp((r - 0.5 * vol_s1 * vol_s1) * dt + vol_s1 * w1[j] * sqrt_dt);
            s2_t = s2_t * exp((r - 0.5 * vol_s2 * vol_s2) * dt + vol_s2 * w2[j] * sqrt_dt);
        }

        final_s1[i] = s1_t;
        final_s2[i] = s2_t;
    }

    return std::pair(final_s1, final_s2);
}

double MCEngine::_computeValue(SpreadOption& option, std::vector<double> final_s1, std::vector<double> final_s2)
{
    double payoffSum = 0.0;
    const double k = option.getStrikePrice();
    const double r = option.getDiscoutRate();
    const double time_to_exp = option.getExpiration();

    for (int i = 0; i < final_s1.size(); ++i)
    {
        payoffSum += std::max(final_s2[i] - final_s1[i] - k, 0.0);
    }

    const double undisc_payoff = payoffSum / num_sim_;
    return undisc_payoff * exp(-r * time_to_exp);
}

std::vector<double> MCEngine::_generateNormalRandomVec()
{
    std::vector<double> rndVec(num_sim_);
    std::generate(rndVec.begin(), rndVec.end(), [&]() {return normal_distribution_(generator_); });

    return rndVec;
}
