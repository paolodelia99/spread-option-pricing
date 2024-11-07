//
// Created by paolodelia on 7/20/24.
//

#ifndef GBMSPREADOPTION_H
#define GBMSPREADOPTION_H
#include "SpreadOption.h"
#include "MCEngine/MCEngine.h"


template<std::floating_point Real>
class GBMSpreadOption : public SpreadOption<Real>
{
public:
    GBMSpreadOption();

    GBMSpreadOption(std::shared_ptr<SpreadMarketData<Real>> spd_mkt, Real strike_price, Real vol_s1, Real vol_s2,
                    Real discount_rate, Real corr);

    ~GBMSpreadOption() override;

    GBMSpreadOption(GBMSpreadOption& other);

    GBMSpreadOption(GBMSpreadOption&& other) noexcept;

    GBMSpreadOption& operator=(GBMSpreadOption& other);

    GBMSpreadOption& operator=(GBMSpreadOption&& other) noexcept;

    Real _getSpreadPrice();

    std::pair<Real, Real> _getDeltas();

    std::pair<Real, Real> _getGammas();

    Real _getCrossGamma();
};



#endif //GBMSPREADOPTION_H
