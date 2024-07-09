//
// Created by paolodelia on 7/9/24.
//

#include "MargrabeOption.h"
#include "MathUtils.h"


MargrabeOption::MargrabeOption(SpreadMarketData* spd_mkt, double vol_s1, double vol_s2, double discount_rate,
    double corr)
        :SpreadOption(spd_mkt, 0.0, vol_s1, vol_s2, discount_rate, corr)
{
}

MargrabeOption::~MargrabeOption()
{
}

float MargrabeOption::getSpreadPrice()
{
    var spot_1 = spd_mkt_->getCurrentAsset1Price();
    var spot_2 = spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = spd_mkt_->getTimeToExpiration();

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, vol_s1_, vol_s2_, corr_);
    return static_cast<float>(price);
}

float MargrabeOption::getCurrentAsset1Price() const
{
    return spd_mkt_->getCurrentAsset1Price();
}

float MargrabeOption::getCurrentAsset2Price() const
{
    return spd_mkt_->getCurrentAsset2Price();
}

std::pair<float, float> MargrabeOption::getDeltas() const
{
    auto [d_s1, d_s2] = _getDeltas();
    return std::pair<float, float>(d_s1, d_s2);
}

std::pair<float, float> MargrabeOption::getGammas() const
{
    auto [dd_s1, dd_s2] = _getGammas();
    return std::pair<float, float>(dd_s1, dd_s2);
}

float MargrabeOption::getCrossGamma() const
{
    return static_cast<float>(_getCrossGamma());
}

var MargrabeOption::_getMargrabePrice(var s1, var s2, var t, var vol_s1, var vol_s2, var corr)
{
    var d1 = computeD1(s1, s2, t, vol_s1, vol_s2, corr);
    var d2 = computeD2(s1, s2, t, vol_s1, vol_s2, corr);

    return s2 * cumulativeNormal(d1) - s1 * cumulativeNormal(d2);
}

std::pair<var, var> MargrabeOption::_getDeltas() const
{
    var spot_1 = spd_mkt_->getCurrentAsset1Price();
    var spot_2 = spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = spd_mkt_->getTimeToExpiration();
    var vol_s1 = vol_s1_;
    var vol_s2 = vol_s2_;
    var corr = corr_;

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, vol_s1, vol_s2, corr);
    auto [d_s1, d_s2] = derivatives(price, wrt(spot_1, spot_2));

    return std::pair<var, var>(d_s1, d_s2);
}

std::pair<var, var> MargrabeOption::_getGammas() const
{
    var spot_1 = spd_mkt_->getCurrentAsset1Price();
    var spot_2 = spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = spd_mkt_->getTimeToExpiration();
    var vol_s1 = vol_s1_;
    var vol_s2 = vol_s2_;
    var corr = corr_;

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, vol_s1, vol_s2, corr);
    auto [d_s1, d_s2] = derivativesx(price, wrt(spot_1, spot_2));

    auto [dd_s1] = derivativesx(d_s1, wrt(spot_1));
    auto [dd_s2] = derivativesx(d_s2, wrt(spot_2));

    return std::pair<var, var>(dd_s1, dd_s2);
}

var MargrabeOption::_getCrossGamma() const
{
    var spot_1 = spd_mkt_->getCurrentAsset1Price();
    var spot_2 = spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = spd_mkt_->getTimeToExpiration();
    var vol_s1 = vol_s1_;
    var vol_s2 = vol_s2_;
    var corr = corr_;

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, vol_s1, vol_s2, corr);
    auto [d_s1, d_s2] = derivativesx(price, wrt(spot_1, spot_2));

    auto [d_s1_d_s2] = derivativesx(d_s1, wrt(spot_2));

    return d_s1_d_s2;
}
