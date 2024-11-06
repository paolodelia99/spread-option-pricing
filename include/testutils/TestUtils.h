//
// Created by paolodelia on 11/6/24.
//

#ifndef TESTUTILS_H
#define TESTUTILS_H
#include <gtest/gtest.h>
#include "GBMSpreadOption.h"
#include "MargrabeOption.h"

template<typename Real>
class BasicExchangeOption : public testing::Test
{
protected:
    BasicExchangeOption() : corr_(0.0), spd_(nullptr) {}

    void SetUp() override
    {
        auto* s1 = new Real(100);
        auto* s2 = new Real(100);
        auto* t = new Real(1.0);
        auto spd = std::make_shared<SpreadMarketData<Real>>(SpreadMarketData(s1, s2, t));

        const Real vol_1 = 0.2;
        const Real vol_2 = vol_1;
        const Real corr = 0.0;
        const Real r = 0.0;

        corr_ = corr;
        spd_ = spd;

        exchange_option_ = std::move(MargrabeOption<Real>(spd, vol_1, vol_2, r, corr));
    }

    void TearDown() override {}

    MargrabeOption<Real> exchange_option_;
    Real corr_;
    std::shared_ptr<SpreadMarketData<Real>> spd_;
};


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
#endif //TESTUTILS_H
