#pragma once

#include "constants.hxx" // pi

#include <random> // std::mt19937, std::random_device, std::uniform_real_distribution

constexpr auto degrees_to_radians(double degrees) -> double { return degrees * pi / 180.0; }

inline auto linear_to_gamma(double linear_component) -> double { return std::sqrt(linear_component); }

inline auto random_double() -> double
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_real_distribution<> distribution(0.0, 1.0);

    return distribution(generator);
}

inline auto random_double(double min, double max) -> double { return min + (max - min) * random_double(); }