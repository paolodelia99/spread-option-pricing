//
// Created by paolodelia on 7/20/24.
//

#include "GBMSpreadOption.h"
#include "MathUtils.h"

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption() : SpreadOption<Real>() {}

template<std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(std::shared_ptr<SpreadMarketData<Real>> spd_mkt, Real strike_price,
    Real vol_s1, Real vol_s2, Real discount_rate, Real corr)
    : SpreadOption<Real>(spd_mkt, strike_price, vol_s1, vol_s2, discount_rate, corr) {}

template<std::floating_point Real>
GBMSpreadOption<Real>::~GBMSpreadOption() {}

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(GBMSpreadOption& other)
:SpreadOption<Real>(other.spd_mkt_, other.strike_price_,
        other.vol_s1_, other.vol_s2_, other.discount_rate_, other.corr_) {}

template <std::floating_point Real>
GBMSpreadOption<Real>::GBMSpreadOption(GBMSpreadOption&& other) noexcept
    : SpreadOption<Real>(other.spd_mkt_, other.strike_price_,
        other.vol_s1_, other.vol_s2_, other.discount_rate_, other.corr_) {}

template <std::floating_point Real>
GBMSpreadOption<Real>& GBMSpreadOption<Real>::operator=(GBMSpreadOption& other)
{
    if (this != &other)
    {
        SpreadOption<Real>::operator=(other);
    }
    return *this;
}

template <std::floating_point Real>
GBMSpreadOption<Real>& GBMSpreadOption<Real>::operator=(GBMSpreadOption&& other) noexcept
{
    if (this != &other)
    {
        SpreadOption<Real>::operator=(std::move(other));
    }
    return *this;
}

inline var KirkApproximation(var s1, var s2, var K, var t, var vol_1, var vol_2, var corr, var r)
{
    var d1 = KirkUtils::computeD1(s1, s2, K, vol_1, vol_2, corr, t);
    var d2 = KirkUtils::computeD2(s1, s2, K, vol_1, vol_2, corr, t);

    return exp(-r * t) * s2 * cumulativeNormal(d1) - (s1 + K) * cumulativeNormal(d2);
}

inline std::pair<var, var> getKirkDeltas(var s1, var s2, var t, var K, var v1, var v2, var corr, var r)
{
    var price = KirkApproximation(s1, s2, K,
        t, v1, v2, corr, r);

    auto [d_s1, d_s2] = derivativesx(price, wrt(s1, s2));

    return std::make_pair(d_s1, d_s2);
}

template <std::floating_point Real>
Real GBMSpreadOption<Real>::_getSpreadPrice()
{
    var spot_1 = this->getCurrentAsset1Price();
    var spot_2 = this->getCurrentAsset2Price();
    var time_to_exp = this->getExpiration();

    var price = KirkApproximation(spot_1, spot_2, this->strike_price_,
        time_to_exp, this->vol_s1_, this->vol_s2_, this->corr_, this->discount_rate_);
    return static_cast<Real>(price);
}

template <std::floating_point Real>
std::pair<Real, Real> GBMSpreadOption<Real>::_getDeltas()
{
    var spot_1 = this->getCurrentAsset1Price();
    var spot_2 = this->getCurrentAsset2Price();
    var time_to_exp = this->getExpiration();
    var strike_price = this->strike_price_;
    var vol_1 = this->getVolAsset1();
    var vol_2 = this->getVolAsset2();
    var corr = this->getCorrelation();
    var r = this->getDiscoutRate();

    auto [d_s1, d_s2] = getKirkDeltas(spot_1, spot_2, time_to_exp,
        strike_price, vol_1, vol_2, corr, r);

    return std::pair<Real, Real>(d_s1, d_s2);
}

template <std::floating_point Real>
std::pair<Real, Real> GBMSpreadOption<Real>::_getGammas()
{
    var spot_1 = this->getCurrentAsset1Price();
    var spot_2 = this->getCurrentAsset2Price();
    var time_to_exp = this->getExpiration();

    auto [d_s1, d_s2] = getKirkDeltas(spot_1, spot_2, time_to_exp,
        this->strike_price_, this->getVolAsset1(), this->getVolAsset2(), this->getCorrelation(),
        this->getDiscoutRate());

    auto [dd_s1] = derivativesx(d_s1, wrt(spot_1));
    auto [dd_s2] = derivativesx(d_s2, wrt(spot_2));

    return std::pair<Real, Real>(dd_s1, dd_s2);
}

template <std::floating_point Real>
Real GBMSpreadOption<Real>::_getCrossGamma()
{
    var spot_1 = this->getCurrentAsset1Price();
    var spot_2 = this->getCurrentAsset2Price();
    var time_to_exp = this->getExpiration();

    auto [d_s1, d_s2] = getKirkDeltas(spot_1, spot_2, time_to_exp,
        this->strike_price_, this->getVolAsset1(), this->getVolAsset2(), this->getCorrelation(),
        this->getDiscoutRate());

    auto [d_s1_d_s2] = derivativesx(d_s1, wrt(spot_2));

    return static_cast<Real>(d_s1_d_s2);
}

template class GBMSpreadOption<float>;
template class GBMSpreadOption<double>;
