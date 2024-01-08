#pragma once

#include "vec3.hxx" // vec3

using point3 = vec3;

class ray
{
public:

    inline ray() = default;

    inline ray(point3 const& origin, vec3 const& direction) : m_origin(origin), m_direction(direction) {}

    inline auto origin() const -> point3 { return m_origin; }

    inline auto direction() const -> vec3 { return m_direction; }

    inline auto at(double t) const -> point3 { return m_origin + t * m_direction; }

private:

    point3 m_origin;
    vec3 m_direction;
};
