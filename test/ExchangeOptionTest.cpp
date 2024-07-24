//
// Created by paolodelia on 7/8/24.
//
#include <gtest/gtest.h>
#include "MargrabeOption.h"

constexpr float TOLERANCE = 0.00001;

class BasicExchangeOptionF : public testing::Test
{
protected:
    BasicExchangeOptionF()
    {
        exchange_option_ = nullptr;
    }

    void SetUp() override
    {
        float* s1 = new float(100);
        float* s2 = new float(100);
        float* t = new float(1.0);
        auto spd = new SpreadMarketData(s1, s2, t);

        const float vol_1 = 0.2;
        const float vol_2 = vol_1;
        const float corr = 0.0;
        const float r = 0.0;

        exchange_option_ = new MargrabeOption(spd, vol_1, vol_2, r, corr);
    }

    void TearDown() override
    {
        delete exchange_option_;
    }

    MargrabeOption<float>* exchange_option_;
};

class BasicExchangeOptionD : public testing::Test
{
protected:
    void SetUp() override
    {
        double* s1 = new double(100);
        double* s2 = new double(100);
        double* t = new double(1.0);
        spd_ = new SpreadMarketData(s1, s2, t);

        const double vol_1 = 0.2;
        const double vol_2 = vol_1;
        const double corr = 0.0;
        const double r = 0.0;

        exchange_option_ = new MargrabeOption(spd_, vol_1, vol_2, r, corr);
    }

    void TearDown() override
    {
        delete exchange_option_;
        delete spd_;
    }

    MargrabeOption<double>* exchange_option_ = nullptr;
    SpreadMarketData<double>* spd_ = nullptr;
};

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
