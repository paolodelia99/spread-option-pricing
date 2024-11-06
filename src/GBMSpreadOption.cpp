//
// Created by paolodelia on 7/20/24.
//

#include "GBMSpreadOption.h"
#include "MathUtils.h"

constexpr unsigned int NUM_SIM = 20000;
constexpr unsigned int ANNUAL_TRADING_DAYS = 252;

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption() : SpreadOption<Real>(), mc_engine_(1000, 252, 4) {}

template<std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(std::shared_ptr<SpreadMarketData<Real>> spd_mkt, Real strike_price,
    Real vol_s1, Real vol_s2, Real discount_rate, Real corr)
    : SpreadOption<Real>(spd_mkt, strike_price, vol_s1, vol_s2, discount_rate, corr),
    mc_engine_(NUM_SIM, static_cast<int>(spd_mkt->getTimeToExpiration() * ANNUAL_TRADING_DAYS), 4) {}

template<std::floating_point Real>
GBMSpreadOption<Real>::~GBMSpreadOption() {}

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
{
    if (this != &other)
    {
        SpreadOption<Real>::operator=(std::move(other));
        mc_engine_ = std::move(other.mc_engine_);
    }
    return *this;
}

inline var _getKirkApproximation(var s1, var s2, var K, var t, var vol_1, var vol_2, var corr, var r)
{
    var d1 = KirkUtils::computeD1(s1, s2, K, vol_1, vol_2, corr, t);
    var d2 = KirkUtils::computeD2(s1, s2, K, vol_1, vol_2, corr, t);

    return exp(-r * t) * s2 * cumulativeNormal(d1) - (s1 + K) * cumulativeNormal(d2);
}

template<std::floating_point Real>
Real GBMSpreadOption<Real>::getSpreadPrice()
{
    var spot_1 = this->spd_mkt_->getCurrentAsset1Price();
    var spot_2 = this->spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = this->spd_mkt_->getTimeToExpiration();

    var price = _getKirkApproximation(spot_1, spot_2, this->strike_price_,
        time_to_exp, this->vol_s1_, this->vol_s2_, this->corr_, this->discount_rate_);
    return static_cast<Real>(price);
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
