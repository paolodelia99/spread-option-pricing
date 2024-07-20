//
// Created by paolodelia on 7/8/24.
//

#ifndef SPREADMARKET_H
#define SPREADMARKET_H

template<typename Real>
struct SpreadMarketData
{
    SpreadMarketData(Real* s1, Real* s2, Real* time_to_exp)
        : s1_(s1),
          s2_(s2),
          time_to_exp_(time_to_exp)
    {
    }

    Real getCurrentAsset1Price() const
    {
        return *s1_;
    }

    Real getCurrentAsset2Price() const
    {
        return *s2_;
    }

    Real getTimeToExpiration() const
    {
        return *time_to_exp_;
    }

private:
    Real* s1_;
    Real* s2_;
    Real* time_to_exp_;
};

template struct SpreadMarketData<float>;
template struct SpreadMarketData<double>;

#endif //SPREADMARKET_H
