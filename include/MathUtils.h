//
// Created by paolodelia on 7/8/24.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H
#include <autodiff/reverse/var.hpp>

using namespace autodiff;

constexpr double SQRT_2 = sqrt(2.0);



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

inline var cumulativeNormal(var x)
{
    return (erf(x / SQRT_2) + 1) / 2;
}

#endif //MATHUTILS_H
