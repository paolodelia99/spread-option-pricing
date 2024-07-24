//
// Created by paolodelia on 7/8/24.
//
#include <gtest/gtest.h>
#include "MargrabeOption.h"

constexpr float TOLERANCE = 0.00001;

template<typename Real>
class BasicExchangeOption : public testing::Test
{
protected:
    BasicExchangeOption()
    {
        exchange_option_ = nullptr;
    }

    void SetUp() override
    {
        auto* s1 = new Real(100);
        auto* s2 = new Real(100);
        auto* t = new Real(1.0);
        auto spd = new SpreadMarketData(s1, s2, t);

        const Real vol_1 = 0.2;
        const Real vol_2 = vol_1;
        const Real corr = 0.0;
        const Real r = 0.0;

        exchange_option_ = new MargrabeOption(spd, vol_1, vol_2, r, corr);
    }

    void TearDown() override
    {
        delete exchange_option_;
    }

    MargrabeOption<Real>* exchange_option_;
};

typedef BasicExchangeOption<float> BasicExchangeOptionF;
typedef BasicExchangeOption<double> BasicExchangeOptionD;

TEST_F(BasicExchangeOptionF, BasicPriceAssertion)
{
    float expected_price = 11.246288299560547;

    EXPECT_NEAR(exchange_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(BasicExchangeOptionF, BasicDeltaAssertion)
{
    float expected_d_s1 = -0.4437684714794159, expected_d_s2 = 0.5562313795089722;
    std::pair<float, float> d_s1_d_s2 = exchange_option_->getDeltas();

    EXPECT_NEAR(d_s1_d_s2.first, expected_d_s1, TOLERANCE);
    EXPECT_NEAR(d_s1_d_s2.second, expected_d_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionF, BasicGammaAssertion)
{
    float expected_dd_s1 = 0.013964394107460976, expected_dd_s2 = 0.013964395970106125;
    std::pair<float, float> dd_s1_dd_s2 = exchange_option_->getGammas();

    EXPECT_NEAR(dd_s1_dd_s2.first, expected_dd_s1, TOLERANCE);
    EXPECT_NEAR(dd_s1_dd_s2.second, expected_dd_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionF, BasicCrossGammaAssertion)
{
    float expected_d_s1_d_s2 = -0.013964394107460976;
    float d_s1_d_s2 = exchange_option_->getCrossGamma();

    EXPECT_NEAR(d_s1_d_s2, expected_d_s1_d_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicPriceAssertion)
{
    const double expected_price = 11.246288299560547;

    EXPECT_NEAR(exchange_option_->getSpreadPrice(), expected_price, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicDeltaAssertion)
{
    double expected_d_s1 = -0.4437684714794159, expected_d_s2 = 0.5562313795089722;
    std::pair<double, double> d_s1_d_s2 = exchange_option_->getDeltas();

    EXPECT_NEAR(d_s1_d_s2.first, expected_d_s1, TOLERANCE);
    EXPECT_NEAR(d_s1_d_s2.second, expected_d_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicGammaAssertion)
{
    double expected_dd_s1 = 0.013964394107460976, expected_dd_s2 = 0.013964395970106125;
    std::pair<double, double> dd_s1_dd_s2 = exchange_option_->getGammas();

    EXPECT_NEAR(dd_s1_dd_s2.first, expected_dd_s1, TOLERANCE);
    EXPECT_NEAR(dd_s1_dd_s2.second, expected_dd_s2, TOLERANCE);
}

TEST_F(BasicExchangeOptionD, BasicCrossGammaAssertion)
{
    double expected_d_s1_d_s2 = -0.013964394107460976;
    double d_s1_d_s2 = exchange_option_->getCrossGamma();

    EXPECT_NEAR(d_s1_d_s2, expected_d_s1_d_s2, TOLERANCE);
}
