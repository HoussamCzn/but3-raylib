#pragma once

#include <array>    // std::array
#include <cmath>    // sqrt
#include <iostream> // std::ostream

class vec3
{
public:

    std::array<double, 3> e{};

    vec3() : e{0, 0, 0} {}

    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    auto x() const -> double { return e[0]; }

    auto y() const -> double { return e[1]; }

    auto z() const -> double { return e[2]; }

    auto length() const -> double { return std::sqrt(length_squared()); }

    auto length_squared() const -> double { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    auto operator-() const -> vec3 { return {-e[0], -e[1], -e[2]}; }

    auto operator[](int i) const -> double { return e[i]; }

    auto operator[](int i) -> double& { return e[i]; }

    auto operator+=(vec3 const& v) -> vec3&
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    auto operator*=(double t) -> vec3&
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    auto operator/=(double t) -> vec3& { return *this *= 1 / t; }
};

// Vector Utility Functions

inline auto operator<<(std::ostream& out, vec3 const& v) -> std::ostream&
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline auto operator+(vec3 const& u, vec3 const& v) -> vec3 { return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]); }

inline auto operator-(vec3 const& u, vec3 const& v) -> vec3 { return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]); }

inline auto operator*(vec3 const& u, vec3 const& v) -> vec3 { return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]); }

inline auto operator*(double t, vec3 const& v) -> vec3 { return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); }

inline auto operator*(vec3 const& v, double t) -> vec3 { return t * v; }

inline auto operator/(vec3 v, double t) -> vec3 { return (1 / t) * v; }

inline auto dot(vec3 const& u, vec3 const& v) -> double { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

inline auto cross(vec3 const& u, vec3 const& v) -> vec3
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline auto unit_vector(vec3 v) -> vec3 { return v / v.length(); }
