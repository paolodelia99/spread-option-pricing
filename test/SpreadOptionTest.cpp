//
// Created by paolodelia on 7/20/24.
//
#include <gtest/gtest.h>
#include "GBMSpreadOption.h"

constexpr float PRICE_TOLERANCE = 0.2;
constexpr float LARGER_PRICE_TOLERANCE = 0.3;

template<typename Real>
class SpdExchangeOption : public testing::Test
{
protected:
    SpdExchangeOption()
        :strike_(0.0), corr_(0.0), spd_(nullptr) {}

    void SetUp() override
    {
        SetUp(0.0, 0.0);
    }

    void SetUp(const Real strike, const Real corr)
    {
        auto* s1 = new Real(100);
        auto* s2 = new Real(100);
        auto* t = new Real(1.0);
        auto spd = std::make_shared<SpreadMarketData<Real>>(SpreadMarketData(s1, s2, t));
        SetUp(spd, strike, corr);
    }

    void SetUp(std::shared_ptr<SpreadMarketData<Real>>spd, const Real strike, const Real corr)
    {
        strike_ = strike;
        corr_ = corr;
        spd_ = spd;
        const Real vol_1 = 0.2;
        const Real vol_2 = vol_1;
        const Real r = 0.0;

        spread_option_ = GBMSpreadOption(spd_, strike_, vol_1, vol_2, r, corr);
    }


    void TearDown() override {}

    GBMSpreadOption<Real> spread_option_;
    Real strike_;
    Real corr_;
    std::shared_ptr<SpreadMarketData<Real>> spd_;
};

using SpdExchangeOptionF = SpdExchangeOption<float>;
using SpdExchangeOptionD = SpdExchangeOption<double>;

TEST_F(SpdExchangeOptionF, BasicExchangePriceAssertion)
{
    constexpr float expected_price = 11.380569442112442;
    float result = spread_option_.getSpreadPrice();

    EXPECT_NEAR(result, expected_price, PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionF, BasicSpreadPriceAssertion)
{
    const float strike = 5.0, corr = 0.0;
    SetUp(strike, corr);
    float expected_price = 9.0117001244117727;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionF, SpreadPriceAssertion)
{
    const float strike = 10.0, corr = 0.0;
    SetUp(strike, corr);
    float expected_price = 7.0182962813092304;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, LARGER_PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionF, SpreadPriceAssertion_1)
{
    const float strike = 10.0, corr = 0.7;
    auto spd = std::make_shared<SpreadMarketData<float>>(SpreadMarketData(new float(100), new float(110), new float(1.0)));
    SetUp(spd ,strike, corr);
    float expected_price = 6.4563;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, LARGER_PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionD, BasicExchangePriceAssertion)
{
    double expected_price = 11.380569442112442;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionD, BasicSpreadPriceAssertion)
{
    const double strike = 5.0, corr = 0.0;
    SetUp(strike, corr);
    double expected_price = 9.0117001244117727;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionD, SpreadPriceAssertion)
{
    const double strike = 10.0, corr = 0.0;
    SetUp(strike, corr);
    double expected_price = 7.0182962813092304;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, PRICE_TOLERANCE);
}

TEST_F(SpdExchangeOptionD, SpreadPriceAssertion_1)
{
    const double strike = 10.0, corr = 0.7;
    auto spd = std::make_shared<SpreadMarketData<double>>(SpreadMarketData(new double(100), new double(110), new double(1.0)));
    SetUp(spd, strike, corr);
    float expected_price = 6.4563;

    EXPECT_NEAR(spread_option_.getSpreadPrice(), expected_price, LARGER_PRICE_TOLERANCE);
}
