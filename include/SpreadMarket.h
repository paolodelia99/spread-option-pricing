//
// Created by paolodelia on 7/8/24.
//

#ifndef SPREADMARKET_H
#define SPREADMARKET_H
#include <concepts>
#include <memory>


template<std::floating_point Real>
struct SpreadMarketData
{
    SpreadMarketData(Real* s1, Real* s2, Real* time_to_exp)
        : s1_(s1),
          s2_(s2),
          time_to_exp_(time_to_exp)
    {
    }

    ~SpreadMarketData() = default;

    SpreadMarketData(const SpreadMarketData& other) = delete;

    SpreadMarketData(SpreadMarketData&& other) noexcept
        :s1_(std::move(other.s1_)),
        s2_(std::move(other.s2_)),
        time_to_exp_(std::move(other.time_to_exp_)) {}

    SpreadMarketData& operator=(const SpreadMarketData& other) = delete;

    SpreadMarketData& operator=(SpreadMarketData&& other) noexcept
    {
        if (this != &other)
        {
            s1_ = std::move(other.s1_);
            s2_ = std::move(other.s2_);
            time_to_exp_ = std::move(other.time_to_exp_);
        }
        return *this;
    }

    [[nodiscard]] Real getCurrentAsset1Price() const
    {
        if (s1_)
            return *s1_;

        return 0.0;
    }

    [[nodiscard]] Real getCurrentAsset2Price() const
    {
        if (s2_)
            return *s2_;

        return 0.0;
    }

    [[nodiscard]] Real getTimeToExpiration() const
    {
        if (time_to_exp_)
            return *time_to_exp_;

        return 0.0;
    }

private:
    std::unique_ptr<Real> s1_;
    std::unique_ptr<Real> s2_;
    std::unique_ptr<Real> time_to_exp_;
};

template struct SpreadMarketData<float>;
template struct SpreadMarketData<double>;

#endif //SPREADMARKET_H
