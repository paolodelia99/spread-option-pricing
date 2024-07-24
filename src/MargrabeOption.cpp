//
// Created by paolodelia on 7/9/24.
//

#include "MargrabeOption.h"
#include "MathUtils.h"


template <typename Real>
MargrabeOption<Real>::MargrabeOption(SpreadMarketData<Real>* spd_mkt, Real vol_s1, Real vol_s2, Real discount_rate,
    Real corr)
        :SpreadOption<Real>(spd_mkt, 0.0, vol_s1, vol_s2, discount_rate, corr)
{
}

template <typename Real>
MargrabeOption<Real>::~MargrabeOption()
{
    delete this->spd_mkt_;
}

template <typename Real>
Real MargrabeOption<Real>::getSpreadPrice()
{
    var spot_1 = this->spd_mkt_->getCurrentAsset1Price();
    var spot_2 = this->spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = this->spd_mkt_->getTimeToExpiration();

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, this->vol_s1_, this->vol_s2_, this->corr_);
    return static_cast<Real>(price);
}

template <typename Real>
Real MargrabeOption<Real>::getCurrentAsset1Price() const
{
    return this->spd_mkt_->getCurrentAsset1Price();
}

template <typename Real>
Real MargrabeOption<Real>::getCurrentAsset2Price() const
{
    return this->spd_mkt_->getCurrentAsset2Price();
}

template <typename Real>
std::pair<Real, Real> MargrabeOption<Real>::getDeltas() const
{
    auto [d_s1, d_s2] = _getDeltas(getCurrentAsset1Price(), getCurrentAsset2Price());
    return std::pair<Real, Real>(d_s1, d_s2);
}

template <typename Real>
std::pair<Real, Real> MargrabeOption<Real>::getGammas() const
{
    auto [dd_s1, dd_s2] = _getGammas(getCurrentAsset1Price(), getCurrentAsset2Price());
    return std::pair<Real, Real>(dd_s1, dd_s2);
}

template <typename Real>
Real MargrabeOption<Real>::getCrossGamma() const
{
    return static_cast<Real>(_getCrossGamma(getCurrentAsset1Price(), getCurrentAsset2Price()));
}

template <typename Real>
var MargrabeOption<Real>::_getMargrabePrice(var s1, var s2, var t, var vol_s1, var vol_s2, var corr)
{
    var d1 = computeD1(s1, s2, t, vol_s1, vol_s2, corr);
    var d2 = computeD2(s1, s2, t, vol_s1, vol_s2, corr);

    return s2 * cumulativeNormal(d1) - s1 * cumulativeNormal(d2);
}

template <typename Real>
std::pair<var, var> MargrabeOption<Real>::_getDeltas(var spot_1, var spot_2) const
{
    var time_to_exp = this->spd_mkt_->getTimeToExpiration();
    var vol_s1 = this->vol_s1_;
    var vol_s2 = this->vol_s2_;
    var corr = this->corr_;

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, vol_s1, vol_s2, corr);
    auto [d_s1, d_s2] = derivativesx(price, wrt(spot_1, spot_2));

    return std::pair<var, var>(d_s1, d_s2);
}

template <typename Real>
std::pair<var, var> MargrabeOption<Real>::_getGammas(var spot_1, var spot_2) const
{
    std::pair<var, var> deltas = _getDeltas(spot_1, spot_2);

    auto [dd_s1] = derivativesx(deltas.first, wrt(spot_1));
    auto [dd_s2] = derivativesx(deltas.second, wrt(spot_2));

    return std::pair<var, var>(dd_s1, dd_s2);
}

template <typename Real>
var MargrabeOption<Real>::_getCrossGamma(var spot_1, var spot_2) const
{
    std::pair<var, var> deltas = _getDeltas(spot_1, spot_2);

    auto [d_s1_d_s2] = derivativesx(deltas.first, wrt(spot_2));

    return d_s1_d_s2;
}

template class MargrabeOption<float>;
template class MargrabeOption<double>;