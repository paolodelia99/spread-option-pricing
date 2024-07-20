//
// Created by paolodelia on 7/20/24.
//

#ifndef GBMSPREADOPTION_H
#define GBMSPREADOPTION_H
#include "SpreadOption.h"
#include "MCEngine.h"


class GBMSpreadOption : public SpreadOption
{
public:
    GBMSpreadOption(SpreadMarketData* spd_mkt, double strike_price, double vol_s1, double vol_s2, double discount_rate,
                double corr);

    ~GBMSpreadOption() override;

    float getSpreadPrice() override;

    std::pair<float, float> getDeltas() const override;

    std::pair<float, float> getGammas() const override;

    float getCrossGamma() const override;

private:
    MCEngine* mc_engine_;
};



#endif //GBMSPREADOPTION_H
