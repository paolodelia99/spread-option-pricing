//
// Created by paolodelia on 7/20/24.
//
#include <gtest/gtest.h>
#include "GBMSpreadOption.h"

constexpr float TOLERANCE = 0.00001;

TEST(GBMSpreadOptionTest, BasicExchangePriceAssertion)
{
    float s1 = 100;
    float s2 = 100;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float K = 0.0;
    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    GBMSpreadOption spd_option(spd, K, vol_1, vol_2, r, corr);
    float expected_price = 11.380569458007812;

    EXPECT_NEAR(spd_option.getSpreadPrice(), expected_price, TOLERANCE);
}

TEST(GBMSpreadOptionTest, BasicSpreadPriceAssertion)
{
    float s1 = 100;
    float s2 = 100;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float K = 5.0;
    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    GBMSpreadOption spd_option(spd, K, vol_1, vol_2, r, corr);
    float expected_price = 9.0117006301879883;

    EXPECT_NEAR(spd_option.getSpreadPrice(), expected_price, TOLERANCE);
}

TEST(GBMSpreadOptionTest, SpreadPriceAssertion)
{
    float s1 = 100;
    float s2 = 110;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float K = 10.0;
    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    GBMSpreadOption spd_option(spd, K, vol_1, vol_2, r, corr);
    float expected_price = 11.98305606842041;

    EXPECT_NEAR(spd_option.getSpreadPrice(), expected_price, TOLERANCE);
}

TEST(GBMSpreadOptionTest, SpreadPriceAssertion_1)
{
    float s1 = 100;
    float s2 = 110;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float K = 10.0;
    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.7;
    const float r = 0.0;

    GBMSpreadOption spd_option(spd, K, vol_1, vol_2, r, corr);
    float expected_price = 6.8752603530883789;

    EXPECT_NEAR(spd_option.getSpreadPrice(), expected_price, TOLERANCE);
}