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

float SpreadOption::getCurrentAsset1Price() const
{
    return spd_mkt_->getCurrentAsset1Price();
}

float SpreadOption::getCurrentAsset2Price() const
{
    return spd_mkt_->getCurrentAsset2Price();
}

double SpreadOption::getCorrelation() const
{
    return corr_;
}

double SpreadOption::getExpiration() const
{
    return spd_mkt_->getTimeToExpiration();
}

double SpreadOption::getDiscoutRate() const
{
    return discount_rate_;
}

double SpreadOption::getVolAsset1() const
{
    return vol_s1_;
}

double SpreadOption::getVolAsset2() const
{
    return vol_s2_;
}

double SpreadOption::getStrikePrice() const
{
    return strike_price_;
}
