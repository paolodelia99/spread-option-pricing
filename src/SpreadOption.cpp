#include "SpreadOption.h"
#include "MathUtils.h"

template<typename Real> SpreadOption<Real>::SpreadOption(SpreadMarketData<Real>* spd_mkt, const Real strike_price, const Real vol_s1, const Real vol_s2,
                           const Real discount_rate, const Real corr)
                : spd_mkt_(spd_mkt),
                strike_price_(strike_price),
                vol_s1_(vol_s1),
                vol_s2_(vol_s2),
                discount_rate_(discount_rate),
                corr_(corr)
{
}

template <typename Real>
Real SpreadOption<Real>::getCurrentAsset1Price()  const
{
    return spd_mkt_->getCurrentAsset1Price();
}

template <typename Real>
Real SpreadOption<Real>::getCurrentAsset2Price() const
{
    return spd_mkt_->getCurrentAsset2Price();
}

template <typename Real>
Real SpreadOption<Real>::getCorrelation() const
{
    return corr_;
}

template <typename Real>
Real SpreadOption<Real>::getExpiration() const
{
    return spd_mkt_->getTimeToExpiration();
}

template <typename Real>
Real SpreadOption<Real>::getDiscoutRate() const
{
    return discount_rate_;
}

template <typename Real>
Real SpreadOption<Real>::getVolAsset1() const
{
    return vol_s1_;
}

template <typename Real>
Real SpreadOption<Real>::getVolAsset2() const
{
    return vol_s2_;
}

template <typename Real>
Real SpreadOption<Real>::getStrikePrice() const
{
    return strike_price_;
}

template class SpreadOption<float>;
template class SpreadOption<double>;