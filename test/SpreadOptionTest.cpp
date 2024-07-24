//
// Created by paolodelia on 7/20/24.
//
#include <gtest/gtest.h>
#include "GBMSpreadOption.h"

constexpr float TOLERANCE = 0.00001;

class SpdExchangeOptionF : public testing::Test
{
protected:
    SpdExchangeOptionF()
        :strike_(0.0), corr_(0.0), spd_(nullptr)
    {
        spread_option_ = nullptr;
    }

    void SetUp() override
    {
        SetUp(0.0, 0.0);
    }

    void SetUp(const float strike, const float corr)
    {
        auto* s1 = new float(100);
        auto* s2 = new float(100);
        auto* t = new float(1.0);
        auto spd = new SpreadMarketData(s1, s2, t);
        SetUp(spd, strike, corr);
    }

    void SetUp(SpreadMarketData<float>* spd, const float strike, const float corr)
    {
        strike_ = strike;
        corr_ = corr;
        spd_ = spd;
        const float vol_1 = 0.2;
        const float vol_2 = vol_1;
        const float r = 0.0;

        spread_option_ = new GBMSpreadOption(spd_, strike_, vol_1, vol_2, r, corr);
    }


    void TearDown() override
    {
        delete spread_option_;
        delete spd_;
    }

    GBMSpreadOption<float>* spread_option_;
    float strike_;
    float corr_;
    SpreadMarketData<float>* spd_;
};

class SpdExchangeOptionD : public testing::Test
{
protected:
    SpdExchangeOptionD()
        :strike_(0.0), corr_(0.0), spd_(nullptr)
    {
        spread_option_ = nullptr;
    }

    void SetUp() override
    {
        SetUp(0.0, 0.0);
    }

    void SetUp(const double strike, const double corr)
    {
        auto* s1 = new double(100);
        auto* s2 = new double(100);
        auto* t = new double(1.0);
        auto spd = new SpreadMarketData(s1, s2, t);
        SetUp(spd, strike, corr);
    }

    void SetUp(SpreadMarketData<double>* spd, const double strike, const double corr)
    {
        strike_ = strike;
        corr_ = corr;
        spd_ = spd;
        const double vol_1 = 0.2;
        const double vol_2 = vol_1;
        const double r = 0.0;

        spread_option_ = new GBMSpreadOption(spd_, strike_, vol_1, vol_2, r, corr);
    }


    void TearDown() override
    {
        delete spread_option_;
        delete spd_;
    }

    GBMSpreadOption<double>* spread_option_;
    double strike_;
    double corr_;
    SpreadMarketData<double>* spd_;
};

TEST_F(SpdExchangeOptionF, BasicExchangePriceAssertion)
{
    float expected_price = 11.485654830932617;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionF, BasicSpreadPriceAssertion)
{
    const float strike = 5.0, corr = 0.0;
    SetUp(strike, corr);
    float expected_price = 9.0753545761108398;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionF, SpreadPriceAssertion)
{
    const float strike = 10.0, corr = 0.0;
    SetUp(strike, corr);
    float expected_price = 7.0478196144104004;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionF, SpreadPriceAssertion_1)
{
    const float strike = 10.0, corr = 0.7;
    SetUp(new SpreadMarketData(new float(100), new float(110), new float(1.0)) ,strike, corr);
    float expected_price = 6.6965165138244629;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionD, BasicExchangePriceAssertion)
{
    double expected_price = 11.380569442112442;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionD, BasicSpreadPriceAssertion)
{
    const double strike = 5.0, corr = 0.0;
    SetUp(strike, corr);
    double expected_price = 9.0117001244117727;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionD, SpreadPriceAssertion)
{
    const double strike = 10.0, corr = 0.0;
    SetUp(strike, corr);
    double expected_price = 7.0182962813092304;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(SpdExchangeOptionD, SpreadPriceAssertion_1)
{
    const double strike = 10.0, corr = 0.7;
    SetUp(new SpreadMarketData(new double(100), new double(110), new double(1.0)) ,strike, corr);
    float expected_price = 6.8752603530883789;

    EXPECT_NEAR(spread_option_->getSpreadPrice(), expected_price, TOLERANCE);
}
