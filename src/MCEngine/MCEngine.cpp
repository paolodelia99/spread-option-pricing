//
// Created by paolodelia on 7/17/24.
//
#include <execution>
#include <future>
#include <random>

#include "MCEngine/MCEngine.h"

MCEngine::MCEngine(unsigned int num_sim, unsigned int n_timesteps, size_t n_threads = 4)
    : n_timesteps_(n_timesteps), num_sim_(num_sim) {}

MCEngine::MCEngine(MCEngine& other_engine)
    : n_timesteps_(other_engine.n_timesteps_), num_sim_(other_engine.num_sim_) {}

MCEngine::MCEngine(MCEngine&& other_engine) noexcept
    :n_timesteps_(other_engine.n_timesteps_), num_sim_(other_engine.num_sim_) {}

MCEngine& MCEngine::operator=(MCEngine& other_engine)
{
    if (this != &other_engine)
    {
        n_timesteps_ = other_engine.n_timesteps_;
        num_sim_ = other_engine.num_sim_;
        //pool_ = std::move(other_engine.pool_);
    }
    return *this;
}

MCEngine& MCEngine::operator=(MCEngine&& other_engine) noexcept
{
    if (this != &other_engine)
    {
        n_timesteps_ = other_engine.n_timesteps_;
        num_sim_ = other_engine.num_sim_;
        //pool_ = std::move(other_engine.pool_);
    }
    return *this;
}

template<std::floating_point Real>
std::vector<Real> createCorrelatedVec(std::vector<Real>& v1, std::vector<Real>& v2, Real rho)
{
    std::vector<Real> res(v1.size());

    std::transform(v1.begin(), v1.end(), v1.begin(), [rho](Real value) {return rho * value; });
    std::transform(v2.begin(), v2.end(), v2.begin(), [rho](Real value) {return sqrt(1 - rho * rho) * value; });

    std::transform(v1.begin(), v1.end(), v2.begin(), res.begin(), std::plus<Real>());

    return res;
}

template <std::floating_point Real>
Real MCEngine::calculatePrice(SpreadOption<Real>& option)
{
    auto [final_s1, final_s2] = _simulatePaths(option);
    return _computeValue(option, final_s1, final_s2);
}

template <std::floating_point Real>
std::pair<std::vector<Real>, std::vector<Real>> MCEngine::_simulatePaths(SpreadOption<Real>& option)
{
    ThreadPool pool(N_THREADS);
    std::vector<Real> final_s1(num_sim_), final_s2(num_sim_);
    std::vector<std::future<std::tuple<Real, Real>>> results(num_sim_);

    const Real dt = option.getExpiration() / n_timesteps_;
    const Real sqrt_dt = sqrt(dt);
    const Real r = option.getDiscoutRate();
    const Real vol_s1 = option.getVolAsset1();
    const Real vol_s2 = option.getVolAsset2();

    auto generateBiGBM = [&] -> std::tuple<Real, Real>
    {
        Real s1_t = option.getCurrentAsset1Price();
        Real s2_t = option.getCurrentAsset2Price();
        std::vector<Real> u1 = _generateNormalRandomVec<Real>();
        std::vector<Real> u2 = _generateNormalRandomVec<Real>();

        std::vector<Real> w1 = std::vector(u1);
        std::vector<Real> w2 = createCorrelatedVec(u1, u2, option.getCorrelation());

        for (int j = 0; j < n_timesteps_; ++j)
        {
            s1_t = s1_t * exp((r - 0.5 * vol_s1 * vol_s1) * dt + vol_s1 * w1[j] * sqrt_dt);
            s2_t = s2_t * exp((r - 0.5 * vol_s2 * vol_s2) * dt + vol_s2 * w2[j] * sqrt_dt);
        }

        return std::make_tuple(s1_t, s2_t);
    };

    for (int i = 0; i < num_sim_; ++i)
    {
        results[i] = pool.enqueue([&]
        {
            return generateBiGBM();
        });
    }

    int i = 0;
    std::for_each(results.begin(), results.end(), [&final_s1, &final_s2, &i](std::future<std::tuple<Real, Real>>& res)
    {
        auto [f_s1, f_s2] = res.get();
        final_s1[i] = f_s1;
        final_s2[i] = f_s2;
        ++i;
    });

    return std::pair(final_s1, final_s2);
}

template <std::floating_point Real>
Real MCEngine::_computeValue(SpreadOption<Real>& option, std::vector<Real>& final_s1, std::vector<Real>& final_s2)
{
    Real payoffSum = 0.0;
    const Real k = option.getStrikePrice();
    const Real r = option.getDiscoutRate();
    const Real time_to_exp = option.getExpiration();
    std::vector<Real> tmp_res(num_sim_);

    std::transform(std::execution::par_unseq, final_s2.begin(), final_s2.end(),
        final_s1.begin(), tmp_res.begin(), [&payoffSum, &k](Real& s2, Real& s1) {
            payoffSum += std::max<Real>(s2 - s1 - k, 0.0);
            return 0.0;
    });

    const Real undisc_payoff = payoffSum / num_sim_;
    return undisc_payoff * exp(-r * time_to_exp);
}

template<std::floating_point Real>
std::vector<Real> MCEngine::_generateNormalRandomVec()
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution<Real> normal_dist(MEAN, STD);

    std::vector<Real> rndVec(num_sim_);
    std::generate(rndVec.begin(), rndVec.end(), [&] {return normal_dist(generator); });

    return rndVec;
}

template float MCEngine::calculatePrice<float>(SpreadOption<float>&);
template double MCEngine::calculatePrice<double>(SpreadOption<double>&);
