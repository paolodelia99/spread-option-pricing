//
// Created by paolodelia on 7/8/24.
//
#include <gtest/gtest.h>
#include "MargrabeOption.h"
#include "testutils/TestUtils.h"

constexpr float TOLERANCE = 0.00001;

using BasicExchangeOptionF = BasicExchangeOption<float>;
using BasicExchangeOptionD = BasicExchangeOption<double>;

TEST_F(BasicExchangeOptionF, BasicPriceAssertion)
{
    float expected_price = 11.246288299560547;

    EXPECT_NEAR(exchange_option_.getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(BasicExchangeOptionF, BasicDeltaAssertion)
{
    float expected_d_s1 = -0.4437684714794159, expected_d_s2 = 0.5562313795089722;
    auto [d_s1, d_s2] = exchange_option_.getDeltas();

    EXPECT_NEAR(d_s1, expected_d_s1, TOLERANCE);
    EXPECT_NEAR(d_s2, expected_d_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionF, BasicGammaAssertion)
{
    float expected_dd_s1 = 0.013964394107460976, expected_dd_s2 = 0.013964395970106125;
    auto [dd_s1, dd_s2] = exchange_option_.getGammas();

    EXPECT_NEAR(dd_s1, expected_dd_s1, TOLERANCE);
    EXPECT_NEAR(dd_s2, expected_dd_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionF, BasicCrossGammaAssertion)
{
    float expected_d_s1_d_s2 = -0.013964394107460976;
    float d_s1_d_s2 = exchange_option_.getCrossGamma();

    EXPECT_NEAR(d_s1_d_s2, expected_d_s1_d_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicPriceAssertion)
{
    const double expected_price = 11.246288299560547;

    EXPECT_NEAR(exchange_option_.getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicDeltaAssertion)
{
    double expected_d_s1 = -0.4437684714794159, expected_d_s2 = 0.5562313795089722;
    auto [d_s1, d_s2] = exchange_option_.getDeltas();

    EXPECT_NEAR(d_s1, expected_d_s1, TOLERANCE);
    EXPECT_NEAR(d_s2, expected_d_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicGammaAssertion)
{
    double expected_dd_s1 = 0.013964394107460976, expected_dd_s2 = 0.013964395970106125;
    auto [dd_s1, dd_s2] = exchange_option_.getGammas();

    EXPECT_NEAR(dd_s1, expected_dd_s1, TOLERANCE);
    EXPECT_NEAR(dd_s2, expected_dd_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicCrossGammaAssertion)
{
    double expected_d_s1_d_s2 = -0.013964394107460976;
    double d_s1_d_s2 = exchange_option_.getCrossGamma();

    EXPECT_NEAR(d_s1_d_s2, expected_d_s1_d_s2, TOLERANCE);
}
