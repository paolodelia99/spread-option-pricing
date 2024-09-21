//
// Created by paolodelia on 7/17/24.
//

#include "MCEngine.h"

template<typename Real>
std::vector<Real> createCorrelatedVec(std::vector<Real>& v1, std::vector<Real>& v2, Real rho)
{
    std::vector<Real> res(v1.size());

    std::transform(v1.begin(), v1.end(), v1.begin(), [rho](Real value) {return rho * value; });
    std::transform(v2.begin(), v2.end(), v2.begin(), [rho](Real value) {return sqrt(1 - rho * rho) * value; });

    std::transform(v1.begin(), v1.end(), v2.begin(), res.begin(), std::plus<Real>());

    return res;
}

template <typename Real>
MCEngine<Real>::MCEngine(unsigned int num_sim, unsigned int n_timesteps, unsigned int seed)
    :num_sim_(num_sim), n_timesteps_(n_timesteps), seed_(seed)
{
    generator_ = std::mt19937(seed_);
    normal_distribution_ = std::normal_distribution<Real>(MEAN, STD);
}

template <typename Real>
Real MCEngine<Real>::operator()(SpreadOption<Real>& option)
{
    auto final_values = _simulatePaths(option);
    return _computeValue(option, final_values.first, final_values.second);
}

template <typename Real>
std::pair<std::vector<Real>, std::vector<Real>> MCEngine<Real>::_simulatePaths(SpreadOption<Real>& option)
{
    std::vector<Real> final_s1(num_sim_), final_s2(num_sim_);

    const Real dt = option.getExpiration() / n_timesteps_;
    const Real sqrt_dt = sqrt(dt);
    const Real r = option.getDiscoutRate();
    const Real vol_s1 = option.getVolAsset1();
    const Real vol_s2 = option.getVolAsset2();

    for (int i = 0; i < num_sim_; ++i)
    {
        Real s1_t = option.getCurrentAsset1Price();
        Real s2_t = option.getCurrentAsset2Price();
        std::vector<Real> u1 = _generateNormalRandomVec();
        std::vector<Real> u2 = _generateNormalRandomVec();

        std::vector<Real> w1 = std::vector(u1);
        std::vector<Real> w2 = createCorrelatedVec(u1, u2, option.getCorrelation());

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

template <typename Real>
Real MCEngine<Real>::_computeValue(SpreadOption<Real>& option, std::vector<Real>& final_s1, std::vector<Real>& final_s2)
{
    Real payoffSum = 0.0;
    const Real k = option.getStrikePrice();
    const Real r = option.getDiscoutRate();
    const Real time_to_exp = option.getExpiration();

    for (int i = 0; i < final_s1.size(); ++i)
    {
        payoffSum += std::max(static_cast<double>(final_s2[i] - final_s1[i] - k), 0.0);
    }

    const Real undisc_payoff = payoffSum / num_sim_;
    return undisc_payoff * exp(-r * time_to_exp);
}

template<typename Real>
std::vector<Real> MCEngine<Real>::_generateNormalRandomVec()
{
    std::vector<Real> rndVec(num_sim_);
    std::generate(rndVec.begin(), rndVec.end(), [&]() {return normal_distribution_(generator_); });

    return rndVec;
}

template class MCEngine<float>;
template class MCEngine<double>;
