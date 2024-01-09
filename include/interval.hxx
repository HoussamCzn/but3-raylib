#pragma once

#include "constants.hxx" // infinity

#include <algorithm> // std::clamp

class interval
{
public:

    constexpr interval() = default;

    constexpr interval(double min, double max) : min(min), max(max) {}

    constexpr auto contains(double value) const -> bool { return min <= value && value <= max; }

    constexpr auto surrounds(double value) const -> bool { return min < value && value < max; }

    constexpr auto clamp(double value) const -> double { return std::clamp(value, min, max); }

    double min{infinity};
    double max{-infinity};
};

static constexpr interval empty{infinity, -infinity};
static constexpr interval universe{-infinity, infinity};