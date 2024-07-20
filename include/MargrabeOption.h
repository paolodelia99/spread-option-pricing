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

template<typename Real>
class MargrabeOption: public SpreadOption<Real>
{
public:
    MargrabeOption(SpreadMarketData<Real>* spd_mkt, Real vol_s1, Real vol_s2, Real discount_rate,
                Real corr);

    ~MargrabeOption();

    Real getSpreadPrice() override;

    Real getCurrentAsset1Price() const;

    Real getCurrentAsset2Price() const;

    std::pair<Real, Real> getDeltas() const override;

    std::pair<Real, Real> getGammas() const override;

    Real getCrossGamma() const override;

private:
    static var _getMargrabePrice(var s1, var s2, var t, var vol_s1, var vol_s2, var corr);
    std::pair<var, var> _getDeltas(var spot_1, var spot_2) const;
    std::pair<var, var> _getGammas(var spot_1, var spot_2) const;
    var _getCrossGamma(var spot_1, var spot_2) const;
};

#endif //MARGRABEOPTION_H
