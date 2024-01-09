#pragma once

#include <array>    // std::array
#include <cmath>    // sqrt
#include <iostream> // std::ostream

class vec3
{
public:

    inline vec3() : e{0, 0, 0} {}

    inline vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    inline auto x() const -> double { return e[0]; }

    inline auto y() const -> double { return e[1]; }

    inline auto z() const -> double { return e[2]; }

    inline auto length() const -> double { return std::sqrt(length_squared()); }

    inline auto length_squared() const -> double { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    inline auto operator-() const -> vec3 { return {-e[0], -e[1], -e[2]}; }

    inline auto operator[](int i) const -> double { return e[i]; }

    inline auto operator[](int i) -> double& { return e[i]; }

    inline auto operator+=(vec3 const& v) -> vec3&
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    inline auto operator*=(double t) -> vec3&
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    inline auto operator/=(double t) -> vec3& { return *this *= 1 / t; }

    friend inline auto operator<<(std::ostream& out, vec3 const& v) -> std::ostream&
    {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    friend inline auto operator+(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
    }

    friend inline auto operator-(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
    }

    friend inline auto operator*(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
    }

    friend inline auto operator*(double t, vec3 const& v) -> vec3 { return {t * v.e[0], t * v.e[1], t * v.e[2]}; }

    friend inline auto operator*(vec3 const& v, double t) -> vec3 { return t * v; }

    friend inline auto operator/(vec3 v, double t) -> vec3 { return (1 / t) * v; }

    std::array<double, 3> e;
};

// Vector Utility Functions

/**
 * @brief Computes the dot product of two vectors.
 * @param u The first vector.
 * @param v The second vector.
 * @return The dot product of the two vectors.
 */
inline auto dot(vec3 const& u, vec3 const& v) -> double { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

/**
 * @brief Computes the cross product of two vectors.
 * @param u The first vector.
 * @param v The second vector.
 * @return The cross product of the two vectors.
 */
inline auto cross(vec3 const& u, vec3 const& v) -> vec3
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

/**
 * @brief Returns a unit vector with the same direction as the given vector.
 * @param v The vector to normalize.
 * @return A unit vector with the same direction as the given vector.
 */
inline auto unit_vector(vec3 v) -> vec3 { return v / v.length(); }