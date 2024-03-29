#pragma once

#include "helpers.hxx" // random_double

#include <algorithm>
#include <array>    // std::array
#include <cmath>    // sqrt
#include <iostream> // std::ostream

class vec3
{
public:

    constexpr vec3() = default;

    constexpr vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    constexpr auto x() const -> double { return e[0]; }

    constexpr auto y() const -> double { return e[1]; }

    constexpr auto z() const -> double { return e[2]; }

    constexpr auto length_squared() const -> double { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    auto length() const -> double { return std::sqrt(length_squared()); }

    auto near_zero() const -> bool
    {
        constexpr auto s = 1e-8;

        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static auto random() -> vec3 { return {random_double(), random_double(), random_double()}; }

    static auto random(double min, double max) -> vec3
    {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }

    constexpr auto operator-() const -> vec3 { return {-e[0], -e[1], -e[2]}; }

    constexpr auto operator[](int i) const -> double { return e[i]; }

    constexpr auto operator[](int i) -> double& { return e[i]; }

    constexpr auto operator+=(vec3 const& v) -> vec3&
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    constexpr auto operator*=(double t) -> vec3&
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    constexpr auto operator/=(double t) -> vec3& { return *this *= 1 / t; }

    friend constexpr auto operator+(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
    }

    friend constexpr auto operator-(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
    }

    friend constexpr auto operator*(vec3 const& u, vec3 const& v) -> vec3
    {
        return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
    }

    friend constexpr auto operator*(double t, vec3 const& v) -> vec3 { return {t * v.e[0], t * v.e[1], t * v.e[2]}; }

    friend constexpr auto operator*(vec3 const& v, double t) -> vec3 { return t * v; }

    friend constexpr auto operator/(vec3 v, double t) -> vec3 { return (1 / t) * v; }

    friend inline auto operator<<(std::ostream& out, vec3 const& v) -> std::ostream&
    {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    std::array<double, 3> e{};
};

/**
 * @brief Computes the dot product of two vectors.
 * @param u The first vector.
 * @param v The second vector.
 * @return The dot product of the two vectors.
 */
constexpr auto dot(vec3 const& u, vec3 const& v) -> double { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

/**
 * @brief Computes the cross product of two vectors.
 * @param u The first vector.
 * @param v The second vector.
 * @return The cross product of the two vectors.
 */
constexpr auto cross(vec3 const& u, vec3 const& v) -> vec3
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

/**
 * @brief Reflects the given vector around the given normal.
 * @param v The vector to reflect.
 * @param n The normal to reflect around.
 * @return The reflected vector.
 */
constexpr auto reflect(vec3 const& v, vec3 const& n) -> vec3 { return v - 2 * dot(v, n) * n; }

/**
 * @brief Refracts the given vector around the given normal.
 * @param uv The vector to refract.
 * @param n The normal to refract around.
 * @param etai_over_etat The ratio of the refractive indices.
 * @return The refracted vector.
 */
inline auto refract(vec3 const& uv, vec3 const& n, double etai_over_etat) -> vec3
{
    auto const cos_theta = std::fmin(dot(-uv, n), 1.0);
    auto const r_out_perp = etai_over_etat * (uv + cos_theta * n);
    auto const r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;

    return r_out_perp + r_out_parallel;
}

/**
 * @brief Returns a unit vector with the same direction as the given vector.
 * @param v The vector to normalize.
 * @return A unit vector with the same direction as the given vector.
 */
inline auto unit_vector(vec3 v) -> vec3 { return v / v.length(); }

/**
 * @brief Returns a random vector with components in the range [min,max).
 * Repeatedly generates random samples until one matches the criteria.
 * @param min The minimum value for each component.
 * @param max The maximum value for each component.
 * @return A random vector with components in the range [min,max).
 */
inline auto random_in_unit_sphere(double min = -1.0, double max = 1.0) -> vec3
{
    for (;;)
    {
        auto const p = vec3::random(min, max);

        if (p.length_squared() < 1) { return p; }
    }
}

inline auto random_unit_vector() -> vec3 { return unit_vector(random_in_unit_sphere()); }

inline auto random_on_hemisphere(vec3 const& normal) -> vec3
{
    auto const in_unit_sphere = random_in_unit_sphere();

    if (dot(in_unit_sphere, normal) > 0.0) { return in_unit_sphere; }
    return -in_unit_sphere;
}
