//
// Created by paolodelia on 7/20/24.
//

#include "GBMSpreadOption.h"

constexpr unsigned int NUM_SIM = 1000;
constexpr unsigned int ANNUAL_TRADING_DAYS = 252;

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption() : SpreadOption<Real>(), mc_engine_(1000, 252, 4) {}

template<std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(std::shared_ptr<SpreadMarketData<Real>> spd_mkt, Real strike_price,
    Real vol_s1, Real vol_s2, Real discount_rate, Real corr)
    : SpreadOption<Real>(spd_mkt, strike_price, vol_s1, vol_s2, discount_rate, corr),
    mc_engine_(NUM_SIM, static_cast<int>(spd_mkt->getTimeToExpiration() * ANNUAL_TRADING_DAYS), 4)
{
    // int n_timesteps = spd_mkt->getTimeToExpiration() * ANNUAL_TRADING_DAYS;
    // mc_engine_ = std::move(MCEngine<Real>(NUM_SIM, n_timesteps, 4));
}

template<std::floating_point Real>
GBMSpreadOption<Real>::~GBMSpreadOption() {}

// template <std::floating_point Real>
// GBMSpreadOption<Real>::GBMSpreadOption(GBMSpreadOption& other)
//     :SpreadOption<Real>(other), mc_engine_(std::move(other.mc_engine_))
// {
// }

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(GBMSpreadOption& other)
:SpreadOption<Real>(other.spd_mkt_, other.strike_price_,
        other.vol_s1_, other.vol_s2_, other.discount_rate_, other.corr_), mc_engine_(other.mc_engine_) {}

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(GBMSpreadOption&& other) noexcept
    : SpreadOption<Real>(other.spd_mkt_, other.strike_price_,
        other.vol_s1_, other.vol_s2_, other.discount_rate_, other.corr_),
    mc_engine_(std::move(other.mc_engine_))
{
}

template <std::floating_point Real>
GBMSpreadOption<Real>& GBMSpreadOption<Real>::operator=(GBMSpreadOption& other)
{
    if (this != &other)
    {
        SpreadOption<Real>::operator=(other);
        mc_engine_ = other.mc_engine_;
    }
    return *this;
}

template <std::floating_point Real>
GBMSpreadOption<Real>& GBMSpreadOption<Real>::operator=(GBMSpreadOption&& other) noexcept
{par_unseq
    if (this != &other)
    {
        SpreadOption<Real>::operator=(std::move(other));
        mc_engine_ = std::move(other.mc_engine_);
    }
    return *this;
}

template<std::floating_point Real>
Real GBMSpreadOption<Real>::getSpreadPrice()
{
    return mc_engine_.calculatePrice<Real>(*this);
}

template<std::floating_point Real>
std::pair<Real, Real> GBMSpreadOption<Real>::getDeltas() const
{
    return std::pair(0.0, 0.0);
}

template<std::floating_point Real>
std::pair<Real, Real> GBMSpreadOption<Real>::getGammas() const
{
    return std::pair(0.0, 0.0);
}

template<std::floating_point Real>
Real GBMSpreadOption<Real>::getCrossGamma() const
{
    return 0.0;
}

template class GBMSpreadOption<float>;
template class GBMSpreadOption<double>;
