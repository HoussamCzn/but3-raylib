#pragma once

#include <array>    // std::array
#include <cmath>    // sqrt
#include <iostream> // std::ostream

class vec3
{
public:

    constexpr vec3() : m_e{0, 0, 0} {}

    constexpr vec3(double e0, double e1, double e2) : m_e{e0, e1, e2} {}

    constexpr auto x() const -> double { return m_e[0]; }

    constexpr auto y() const -> double { return m_e[1]; }

    constexpr auto z() const -> double { return m_e[2]; }

    constexpr auto length() const -> double { return std::sqrt(length_squared()); }

    constexpr auto length_squared() const -> double { return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2]; }

    constexpr auto operator-() const -> vec3 { return {-m_e[0], -m_e[1], -m_e[2]}; }

    constexpr auto operator[](int i) const -> double { return m_e[i]; }

    constexpr auto operator[](int i) -> double& { return m_e[i]; }

    constexpr auto operator+=(vec3 const& v) -> vec3&
    {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        m_e[2] += v.m_e[2];

        return *this;
    }

    constexpr auto operator*=(double t) -> vec3&
    {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;

        return *this;
    }

    constexpr auto operator/=(double t) -> vec3& { return *this *= 1 / t; }

    friend constexpr auto operator<<(std::ostream& out, vec3 const& v) -> std::ostream&
    {
        return out << v.m_e[0] << ' ' << v.m_e[1] << ' ' << v.m_e[2];
    }

    friend constexpr auto operator+(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.m_e[0] + v.m_e[0], u.m_e[1] + v.m_e[1], u.m_e[2] + v.m_e[2]};
    }

    friend constexpr auto operator-(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.m_e[0] - v.m_e[0], u.m_e[1] - v.m_e[1], u.m_e[2] - v.m_e[2]};
    }

    friend constexpr auto operator*(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.m_e[0] * v.m_e[0], u.m_e[1] * v.m_e[1], u.m_e[2] * v.m_e[2]};
    }

    friend constexpr auto operator*(double t, vec3 const& v) -> vec3 { return {t * v.m_e[0], t * v.m_e[1], t * v.m_e[2]}; }

    friend constexpr auto operator*(vec3 const& v, double t) -> vec3 { return t * v; }

    friend constexpr auto operator/(vec3 v, double t) -> vec3 { return (1 / t) * v; }

private:

    std::array<double, 3> m_e;
};

// Vector Utility Functions

constexpr auto dot(vec3 const& u, vec3 const& v) -> double { return u.x() * v.x() + u.y() * v.y() + u.z() * v.z(); }

constexpr auto cross(vec3 const& u, vec3 const& v) -> vec3
{
    return {u.x() * v.y() - u.y() * v.x(), u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z()};
}

constexpr auto unit_vector(vec3 v) -> vec3 { return v / v.length(); }
