#pragma once

#include "constants.hxx" // infinity

constexpr auto degrees_to_radians(double degrees) -> double { return degrees * pi / 180.0; }
