#ifndef SPREAD_PRICING_LIBRARY_H
#define SPREAD_PRICING_LIBRARY_H
#include "SpreadMarket.h"
#include "utility"
#include <autodiff/reverse/var.hpp>

using namespace autodiff;

template<typename Real>
class SpreadOption
{
    static_assert(std::is_floating_point<Real>::value, "Real must be a floating point type");

public:
    SpreadOption(SpreadMarketData<Real>* spd_mkt, Real strike_price, Real vol_s1, Real vol_s2, Real discount_rate,
                Real corr);

    virtual ~SpreadOption() = default;

    Real getCurrentAsset1Price() const;

    Real getCurrentAsset2Price() const;

    Real getCorrelation() const;

    Real getExpiration() const;

    Real getDiscoutRate() const;

    Real getVolAsset1() const;

    Real getVolAsset2() const;

    Real getStrikePrice() const;

    virtual Real getSpreadPrice() = 0;

    virtual std::pair<Real, Real> getDeltas() const = 0;

    virtual std::pair<Real, Real> getGammas() const = 0;

    virtual Real getCrossGamma() const = 0;

protected:
    SpreadMarketData<Real>* spd_mkt_;
    const Real strike_price_;
    const Real vol_s1_;
    const Real vol_s2_;
    const Real discount_rate_;
    const Real corr_;
};

#endif //SPREAD_PRICING_LIBRARY_H
