#ifndef SPREAD_PRICING_LIBRARY_H
#define SPREAD_PRICING_LIBRARY_H
#include "SpreadMarket.h"
#include "utility"
#include <autodiff/reverse/var.hpp>

using namespace autodiff;

class SpreadOption
{
public:
    SpreadOption(SpreadMarketData* spd_mkt, double strike_price, double vol_s1, double vol_s2, double discount_rate,
                double corr);

    ~SpreadOption() = default;

    virtual float getSpreadPrice() = 0;

    float getCurrentAsset1Price() const;

    float getCurrentAsset2Price() const;

    double getCorrelation() const;

    double getExpiration() const;

    double getDiscoutRate() const;

    double getVolAsset1() const;

    double getVolAsset2() const;

    double getStrikePrice() const;

    virtual std::pair<float, float> getDeltas() const = 0;

    virtual std::pair<float, float> getGammas() const = 0;

    virtual float getCrossGamma() const = 0;

protected:
    SpreadMarketData* spd_mkt_;
    const double strike_price_;
    const double vol_s1_;
    const double vol_s2_;
    const double discount_rate_;
    const double corr_;
};

#endif //SPREAD_PRICING_LIBRARY_H
