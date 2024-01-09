#pragma once

#include "color.hxx"    // color
#include "hittable.hxx" // hit_record
#include "ray.hxx"      // ray

class material
{
public:

    virtual ~material() = default;

    virtual auto scatter(ray const& r_in, hit_record const& rec, color& attenuation, ray& scattered) const -> bool = 0;
};

class lambertian : public material
{
public:

    explicit lambertian(color const& a) : m_albedo(a) {}

    auto scatter(ray const& /* r_in */, hit_record const& rec, color& attenuation, ray& scattered) const -> bool override
    {
        auto scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero()) { scatter_direction = rec.normal; }

        scattered = ray(rec.p, scatter_direction);
        attenuation = m_albedo;

        return true;
    }

private:

    color m_albedo;
};

class metal : public material
{
public:

    explicit metal(color const& a, double const fuzz) : m_albedo(a), m_fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

    auto scatter(ray const& r_in, hit_record const& rec, color& attenuation, ray& scattered) const -> bool override
    {
        auto const reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + m_fuzz * random_in_unit_sphere());
        attenuation = m_albedo;

        return (dot(scattered.direction(), rec.normal) > 0.0);
    }

private:

    color m_albedo;
    double m_fuzz;
};