#pragma once

#include "constants.hxx" // infinity

class interval
{
public:

    constexpr interval() = default;

    constexpr interval(double min, double max) : min(min), max(max) {}

    auto contains(double value) const -> bool { return min <= value && value <= max; }

    auto surrounds(double value) const -> bool { return min < value && value < max; }

    double min{infinity};
    double max{-infinity};
};

static constexpr interval empty{infinity, -infinity};
static constexpr interval universe{-infinity, infinity};