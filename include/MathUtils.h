//
// Created by paolodelia on 7/8/24.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H
#include <autodiff/reverse/var.hpp>

using namespace autodiff;

const double SQRT_2 = sqrt(2.0);

inline var cumulativeNormal(var x)
{
    return (erf(x / SQRT_2) + 1) / 2;
}

namespace MargrabeUtils {
    inline var computeD1(var spot_1, var spot_2, var time_to_exp, var sigma_1, var sigma_2, var corr)
    {
        var sigma = sqrt(pow(sigma_1, 2) + pow(sigma_2, 2) - 2 * corr * sigma_1 * sigma_2);
        var sigma_sqrt_t = sigma * sqrt(time_to_exp);

        return (log(spot_2 / spot_1) / sigma_sqrt_t) + 0.5 * sigma_sqrt_t;
    }

    inline var computeD2(var spot_1, var spot_2, var time_to_exp, var sigma_1, var sigma_2, var corr)
    {
        var sigma = sqrt(pow(sigma_1, 2) + pow(sigma_2, 2) - 2 * corr * sigma_1 * sigma_2);
        var sigma_sqrt_t = sigma * sqrt(time_to_exp);

        var d1 = (log(spot_2 / spot_1) / sigma_sqrt_t) + 0.5 * sigma_sqrt_t;

        return d1 - sigma_sqrt_t;
    }
}

namespace KirkUtils
{
    inline var computeAt(var sigma_1, var sigma_2, var spot_1, var strike, var corr)
    {
        var sigma_1_sq = pow(sigma_1, 2);
        var sigma_2_sq = pow(sigma_2, 2);
        var s_t_ratio = spot_1 / (spot_1 + strike);
        var s_t_ratio_sq = pow(spot_1, 2) / pow(spot_1 + strike, 2);

        return sqrt(sigma_1_sq - 2 * corr * sigma_1 * sigma_2 * s_t_ratio + sigma_2_sq * s_t_ratio_sq);
    }

    inline var computeApproxIV(var a_t, var spot_1, var spot_2, var sigma_1, var sigma_2, var strike, var corr)
    {
        var X_t = log(spot_2);
        var x_t = log(spot_1 - strike);
        var sqrt_a_t_sq = sqrt(pow(a_t, 2));
        var at_ratio = 1 / pow(sqrt_a_t_sq, 3);
        var s1_ratio = spot_1 / (spot_1 + strike);
        var s1_ratio_sq = (spot_1 * strike) / pow(spot_1 + strike ,2);

        return sqrt_a_t_sq + 0.5 * pow(sigma_2 * s1_ratio - corr * sigma_1, 2) * at_ratio * pow(sigma_2, 2) *
            s1_ratio_sq * (X_t - x_t);
    }

    inline var computeD1(var spot_1, var spot_2, var strike, var sigma_1, var sigma_2, var corr, var time_to_exp)
    {
        var a_t = computeAt(sigma_1, sigma_2, spot_1, strike, corr);
        var S = spot_2 / (spot_1 + strike);
        var I_v = computeApproxIV(a_t, spot_1, spot_2, sigma_1, sigma_2, strike, corr);

        return (log(S) + 0.5 * pow(I_v, 2) * time_to_exp) / (I_v * sqrt(time_to_exp));
    }

    inline var computeD2(var spot_1, var spot_2, var strike, var sigma_1, var sigma_2, var corr, var time_to_exp)
    {
        var a_t = computeAt(sigma_1, sigma_2, spot_1, strike, corr);
        var d1 = computeD1(spot_1, spot_2, strike, sigma_1, sigma_2, corr, time_to_exp);
        var I_v = computeApproxIV(a_t, spot_1, spot_2, sigma_1, sigma_2, strike, corr);

        return d1 - I_v * sqrt(time_to_exp);
    }
}

#endif //MATHUTILS_H
