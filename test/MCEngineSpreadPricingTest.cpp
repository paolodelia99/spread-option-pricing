//
// Created by paolodelia on 11/6/24.
//
#ifdef DEBUG
#define SKIP_TEST(reason) GTEST_SKIP_(reason)
#else
#define SKIP_TEST(reason)
#endif

#include <gtest/gtest.h>
#include "GBMSpreadOption.h"
#include "MCEngine/MCEngine.h"
#include "testutils/TestUtils.h"

constexpr float PRICE_TOLERANCE = 0.3;
constexpr int N_SIM = 20000;
constexpr int ANNUAL_TRADING_DAYS = 252;

using MCEngineSpreadOptF = SpdExchangeOption<float>;
using MCEngineSpreadOptD = SpdExchangeOption<double>;

TEST_F(MCEngineSpreadOptF, BasicExchangePriceAssertion)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    constexpr float expected_price = 11.145873069763184;
    float result = mc_engine.calculatePrice(spread_option_);

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptF, BasicSpreadPriceAssertion)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    const float strike = 5.0, corr = 0.0;
    SetUp(strike, corr);
    float expected_price = 9.0117001244117727;
    float result = mc_engine.calculatePrice(spread_option_);

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptF, SpreadPriceAssertion)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    const float strike = 10.0, corr = 0.0;
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    SetUp(strike, corr);
    float expected_price = 7.0182962813092304;
    float result = mc_engine.calculatePrice(spread_option_);

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptF, SpreadPriceAssertion_1)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    const float strike = 10.0, corr = 0.7;
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    auto spd = std::make_shared<SpreadMarketData<float>>(SpreadMarketData(new float(100), new float(110), new float(1.0)));
    SetUp(spd ,strike, corr);
    float expected_price = 6.4563;
    float result = mc_engine.calculatePrice<float>(spread_option_);

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptD, BasicExchangePriceAssertion)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    double expected_price = 11.145873069763184;
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    double result = mc_engine.calculatePrice(spread_option_);
    
    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptD, BasicSpreadPriceAssertion)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    const double strike = 5.0, corr = 0.0;
    SetUp(strike, corr);
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    double result = mc_engine.calculatePrice(spread_option_);
    double expected_price = 9.0117001244117727;

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptD, SpreadPriceAssertion)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    const double strike = 10.0, corr = 0.0;
    SetUp(strike, corr);
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    double result = mc_engine.calculatePrice(spread_option_);
    double expected_price = 7.0182962813092304;

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(MCEngineSpreadOptD, SpreadPriceAssertion_1)
{
    SKIP_TEST("Skipping long-running test in debug mode");
    const double strike = 10.0, corr = 0.7;
    auto spd = std::make_shared<SpreadMarketData<double>>(SpreadMarketData(new double(100), new double(110), new double(1.0)));
    SetUp(spd, strike, corr);
    MCEngine mc_engine(N_SIM, ANNUAL_TRADING_DAYS);
    double result = mc_engine.calculatePrice(spread_option_);
    float expected_price = 6.4563;

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}
