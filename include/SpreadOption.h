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

    ~SpreadOption();

    float getSpreadPrice();

    float getCurrentAsset1Price() const;

    float getCurrentAsset2Price() const;

    std::pair<float, float> getDeltas() const;

    std::pair<float, float> getGammas() const;

private:
    static var _getMargrabePrice(var s1, var s2, var t, var vol_s1, var vol_s2, var corr);
    std::pair<var, var> _getDeltas() const;
    std::pair<var, var> _getGammas() const;

    SpreadMarketData* spd_mkt_;
    const double strike_price_;
    const double vol_s1_;
    const double vol_s2_;
    const double discount_rate_;
    const double corr_;
};

#endif //SPREAD_PRICING_LIBRARY_H
