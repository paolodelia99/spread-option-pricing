//
// Created by paolodelia on 7/9/24.
//

#ifndef MARGRABEOPTION_H
#define MARGRABEOPTION_H

#include "SpreadOption.h"
#include "SpreadMarket.h"
#include "utility"
#include <autodiff/reverse/var.hpp>

using namespace autodiff;

class MargrabeOption: public SpreadOption
{
public:
    MargrabeOption(SpreadMarketData* spd_mkt, double vol_s1, double vol_s2, double discount_rate,
                double corr);

    ~MargrabeOption();

    float getSpreadPrice() override;

    float getCurrentAsset1Price() const;

    float getCurrentAsset2Price() const;

    std::pair<float, float> getDeltas() const override;

    std::pair<float, float> getGammas() const override;

    float getCrossGamma() const override;

private:
    static var _getMargrabePrice(var s1, var s2, var t, var vol_s1, var vol_s2, var corr);
    std::pair<var, var> _getDeltas() const;
    std::pair<var, var> _getGammas() const;
    var _getCrossGamma() const;
};

#endif //MARGRABEOPTION_H
