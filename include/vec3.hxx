#pragma once

#include <array>    // std::array
#include <cmath>    // sqrt
#include <iostream> // std::ostream

class vec3
{
public:

    constexpr vec3() : m_e{0, 0, 0} {}

    constexpr vec3(double e0, double e1, double e2) : m_e{e0, e1, e2} {}

    auto x() const -> double { return m_e[0]; }

    auto y() const -> double { return m_e[1]; }

    auto z() const -> double { return m_e[2]; }

    auto length() const -> double { return std::sqrt(length_squared()); }

    auto length_squared() const -> double { return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2]; }

    auto operator-() const -> vec3 { return {-m_e[0], -m_e[1], -m_e[2]}; }

    auto operator[](int i) const -> double { return m_e[i]; }

    auto operator[](int i) -> double& { return m_e[i]; }

    auto operator+=(vec3 const& v) -> vec3&
    {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        m_e[2] += v.m_e[2];

        return *this;
    }

    auto operator*=(double t) -> vec3&
    {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;

        return *this;
    }

    auto operator/=(double t) -> vec3& { return *this *= 1 / t; }

    friend inline auto operator<<(std::ostream& out, vec3 const& v) -> std::ostream&
    {
        return out << v.m_e[0] << ' ' << v.m_e[1] << ' ' << v.m_e[2];
    }

    friend inline auto operator+(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.m_e[0] + v.m_e[0], u.m_e[1] + v.m_e[1], u.m_e[2] + v.m_e[2]};
    }

    friend inline auto operator-(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.m_e[0] - v.m_e[0], u.m_e[1] - v.m_e[1], u.m_e[2] - v.m_e[2]};
    }

    friend inline auto operator*(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.m_e[0] * v.m_e[0], u.m_e[1] * v.m_e[1], u.m_e[2] * v.m_e[2]};
    }

    friend inline auto operator*(double t, vec3 const& v) -> vec3 { return {t * v.m_e[0], t * v.m_e[1], t * v.m_e[2]}; }

    friend inline auto operator*(vec3 const& v, double t) -> vec3 { return t * v; }

    friend inline auto operator/(vec3 v, double t) -> vec3 { return (1 / t) * v; }

private:

    std::array<double, 3> m_e;
};

// Vector Utility Functions

inline auto dot(vec3 const& u, vec3 const& v) -> double { return u.x() * v.x() + u.y() * v.y() + u.z() * v.z(); }

inline auto cross(vec3 const& u, vec3 const& v) -> vec3
{
    return {u.x() * v.y() - u.y() * v.x(), u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z()};
}

inline auto unit_vector(vec3 v) -> vec3 { return v / v.length(); }
