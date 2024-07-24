//
// Created by paolodelia on 7/20/24.
//

#include "GBMSpreadOption.h"

constexpr unsigned int NUM_SIM = 1000;
constexpr unsigned int ANNUAL_TRADING_DAYS = 252;
constexpr unsigned int SEED = 0;

template<typename Real>
GBMSpreadOption<Real>::GBMSpreadOption(SpreadMarketData<Real>* spd_mkt, Real strike_price, Real vol_s1, Real vol_s2,
    Real discount_rate, Real corr): SpreadOption<Real>(spd_mkt, strike_price, vol_s1, vol_s2, discount_rate, corr)
{
    int n_timesteps = spd_mkt->getTimeToExpiration() * ANNUAL_TRADING_DAYS;
    mc_engine_ = new MCEngine<Real>(NUM_SIM, n_timesteps, SEED);
}

template<typename Real>
GBMSpreadOption<Real>::~GBMSpreadOption()
{
    delete mc_engine_;
}

template<typename Real>
Real GBMSpreadOption<Real>::getSpreadPrice()
{
    return mc_engine_->operator()(*this);
}

template<typename Real>
std::pair<Real, Real> GBMSpreadOption<Real>::getDeltas() const
{
    return std::pair(0.0, 0.0);
}

template<typename Real>
std::pair<Real, Real> GBMSpreadOption<Real>::getGammas() const
{
    return std::pair(0.0, 0.0);
}

template<typename Real>
Real GBMSpreadOption<Real>::getCrossGamma() const
{
    return 0.0;
}

template class GBMSpreadOption<float>;
template class GBMSpreadOption<double>;
