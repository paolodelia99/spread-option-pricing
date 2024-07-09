//
// Created by paolodelia on 7/8/24.
//

#ifndef SPREADMARKET_H
#define SPREADMARKET_H

struct SpreadMarketData
{
    SpreadMarketData(float* s1, float* s2, float* time_to_exp)
        : s1_(s1),
          s2_(s2),
          time_to_exp_(time_to_exp)
    {
    }

    float getCurrentAsset1Price() const
    {
        return *s1_;
    }

    float getCurrentAsset2Price() const
    {
        return *s2_;
    }

    float getTimeToExpiration() const
    {
        return *time_to_exp_;
    }

private:
    float* s1_;
    float* s2_;
    float* time_to_exp_;
};

#endif //SPREADMARKET_H
