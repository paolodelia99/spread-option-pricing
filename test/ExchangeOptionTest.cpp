//
// Created by paolodelia on 7/8/24.
//
#include <gtest/gtest.h>
#include "MargrabeOption.h"

constexpr float TOLERANCE = 0.00001;

TEST(ExchangeOptionTest, BasicPriceAssertion)
{
    float s1 = 100;
    float s2 = 100;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    MargrabeOption spd_option(spd, vol_1, vol_2, r, corr);
    float expected_price = 11.246288299560547;

    EXPECT_NEAR(spd_option.getSpreadPrice(), expected_price, TOLERANCE);
}

TEST(ExchangeOptionTest, BasicDeltaAssertion)
{
    float s1 = 100;
    float s2 = 100;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    MargrabeOption spd_option(spd, vol_1, vol_2, r, corr);
    float expected_d_s1 = -0.4437684714794159, expected_d_s2 = 0.5562313795089722;
    std::pair<float, float> d_s1_d_s2 = spd_option.getDeltas();

    EXPECT_NEAR(d_s1_d_s2.first, expected_d_s1, TOLERANCE);
    EXPECT_NEAR(d_s1_d_s2.second, expected_d_s2, TOLERANCE);
}

TEST(ExchangeOptionTest, BasicGammaAssertion)
{
    float s1 = 100;
    float s2 = 100;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    MargrabeOption spd_option(spd, vol_1, vol_2, r, corr);
    float expected_dd_s1 = 0.013964394107460976, expected_dd_s2 = 0.013964395970106125;
    std::pair<float, float> dd_s1_dd_s2 = spd_option.getGammas();

    EXPECT_NEAR(dd_s1_dd_s2.first, expected_dd_s1, TOLERANCE);
    EXPECT_NEAR(dd_s1_dd_s2.second, expected_dd_s2, TOLERANCE);
}

TEST(ExchangeOptionTest, BasicCrossGammaAssertion)
{
    float s1 = 100;
    float s2 = 100;
    float t = 1.0;
    auto spd = new SpreadMarketData(&s1, &s2, &t);

    const float vol_1 = 0.2;
    const float vol_2 = vol_1;
    const float corr = 0.0;
    const float r = 0.0;

    MargrabeOption spd_option(spd, vol_1, vol_2, r, corr);
    float expected_d_s1_d_s2 = -0.013964394107460976;
    float d_s1_d_s2 = spd_option.getCrossGamma();

    EXPECT_NEAR(d_s1_d_s2, expected_d_s1_d_s2, TOLERANCE);
}