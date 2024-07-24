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

    ~SpreadMarketData()
    {
        s1_ = nullptr;
        s2_ = nullptr;
        time_to_exp_ = nullptr;
    }

    SpreadMarketData(const SpreadMarketData& other)
        :s1_(new Real(other.getCurrentAsset1Price())), s2_(new Real(other.getCurrentAsset2Price())),
        time_to_exp_(new Real(other.getTimeToExpiration()))
    {
    }

    SpreadMarketData& operator=(const SpreadMarketData& other)
    {
        if (this != &other)
        {
            delete s1_;
            delete s2_;
            delete time_to_exp_;
            s1_ = other.s1_ ? new Real(*(other.s1_)): nullptr;
            s2_ = other.s2_ ? new Real(*(other.s2_)): nullptr;
            time_to_exp_ = other.time_to_exp_ ? new Real(*(other.time_to_exp_)): nullptr;
        }
        return *this;
    }

    Real getCurrentAsset1Price() const
    {
        if (s1_)
            return *s1_;

        return 0.0;
    }

    Real getCurrentAsset2Price() const
    {
        if (s2_)
            return *s2_;

        return 0.0;
    }

    Real getTimeToExpiration() const
    {
        if (time_to_exp_)
            return *time_to_exp_;

        return 0.0;
    }

private:
    Real* s1_;
    Real* s2_;
    Real* time_to_exp_;
};

template struct SpreadMarketData<float>;
template struct SpreadMarketData<double>;

#endif //SPREADMARKET_H
