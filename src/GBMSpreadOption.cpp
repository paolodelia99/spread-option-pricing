//
// Created by paolodelia on 7/20/24.
//

#include "GBMSpreadOption.h"

constexpr unsigned int NUM_SIM = 1000;
constexpr unsigned int ANNUAL_TRADING_DAYS = 252;
constexpr unsigned int SEED = 0;

GBMSpreadOption::GBMSpreadOption(SpreadMarketData* spd_mkt, double strike_price, double vol_s1, double vol_s2,
    double discount_rate, double corr): SpreadOption(spd_mkt, strike_price, vol_s1, vol_s2, discount_rate, corr)
{
    int n_timesteps = spd_mkt->getTimeToExpiration() * ANNUAL_TRADING_DAYS;
    mc_engine_ = new MCEngine(NUM_SIM, n_timesteps, SEED);
}

GBMSpreadOption::~GBMSpreadOption()
{
    delete mc_engine_;
}

float GBMSpreadOption::getSpreadPrice()
{
    return mc_engine_->operator()(*this);
}

std::pair<float, float> GBMSpreadOption::getDeltas() const
{
    return std::pair(0.0, 0.0);
}

std::pair<float, float> GBMSpreadOption::getGammas() const
{
    return std::pair(0.0, 0.0);
}

float GBMSpreadOption::getCrossGamma() const
{
    return 0.0;
}


