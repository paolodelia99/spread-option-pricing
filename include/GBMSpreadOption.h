//
// Created by paolodelia on 7/20/24.
//

#ifndef GBMSPREADOPTION_H
#define GBMSPREADOPTION_H
#include "SpreadOption.h"
#include "MCEngine.h"


template<typename Real>
class GBMSpreadOption : public SpreadOption<Real>
{
public:
    GBMSpreadOption(SpreadMarketData<Real>* spd_mkt, Real strike_price, Real vol_s1, Real vol_s2, Real discount_rate,
                Real corr);

    ~GBMSpreadOption() override;

    Real getSpreadPrice() override;

    std::pair<Real, Real> getDeltas() const override;

    std::pair<Real, Real> getGammas() const override;

    Real getCrossGamma() const override;

private:
    MCEngine<Real>* mc_engine_;
};



#endif //GBMSPREADOPTION_H
