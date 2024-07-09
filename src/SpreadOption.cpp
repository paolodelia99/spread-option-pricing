#include "SpreadOption.h"
#include "MathUtils.h"

SpreadOption::SpreadOption(SpreadMarketData* spd_mkt, const double strike_price, const double vol_s1, const double vol_s2,
                           const double discount_rate, const double corr)
                : spd_mkt_(spd_mkt),
                strike_price_(strike_price),
                vol_s1_(vol_s1),
                vol_s2_(vol_s2),
                discount_rate_(discount_rate),
                corr_(corr)
{
}

SpreadOption::~SpreadOption()
{
}

float SpreadOption::getSpreadPrice()
{
    var spot_1 = spd_mkt_->getCurrentAsset1Price();
    var spot_2 = spd_mkt_->getCurrentAsset2Price();
    var time_to_exp = spd_mkt_->getTimeToExpiration();

    var price = _getMargrabePrice(spot_1, spot_2, time_to_exp, vol_s1_, vol_s2_, corr_);
    return static_cast<float>(price);
}

float SpreadOption::getCurrentAsset1Price() const
{
    return spd_mkt_->getCurrentAsset1Price();
}

float SpreadOption::getCurrentAsset2Price() const
{
    return spd_mkt_->getCurrentAsset2Price();
}

std::pair<float, float> SpreadOption::getDeltas() const
{
    auto [d_s1, d_s2] = _getDeltas();
    return std::pair<float, float>(d_s1, d_s2);
}

std::pair<float, float> SpreadOption::getGammas() const
{
    auto [dd_s1, dd_s2] = _getGammas();
    return std::pair<float, float>(dd_s1, dd_s2);
}

float SpreadOption::getCrossGamma() const
{
    return static_cast<float>(_getCrossGamma());
}

var SpreadOption::_getMargrabePrice(var s1, var s2, var t, var vol_s1, var vol_s2, var corr)
{
    var d1 = computeD1(s1, s2, t, vol_s1, vol_s2, corr);
    var d2 = computeD2(s1, s2, t, vol_s1, vol_s2, corr);

    return s2 * cumulativeNormal(d1) - s1 * cumulativeNormal(d2);
}

std::pair<var, var> SpreadOption::_getDeltas() const
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

std::pair<var, var> SpreadOption::_getGammas() const
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

var SpreadOption::_getCrossGamma() const
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
