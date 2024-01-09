#pragma once

#include <cmath>
#include <limits>
#include <memory>

static constexpr auto infinity = std::numeric_limits<double>::infinity();
static constexpr auto pi = 3.1415926535897932385;

constexpr auto degrees_to_radians(double degrees) -> double { return degrees * pi / 180.0; }

#include "ray.hxx"
#include "vec3.hxx"
