#pragma once

#include "color.hxx"    // color
#include "hittable.hxx" // hit_record
#include "ray.hxx"      // ray

class material
{
public:

    virtual ~material() = default;

    virtual auto get_color() const -> color = 0;

    virtual auto scatter(ray const& r_in, hit_record const& rec, color& attenuation, ray& scattered) const -> bool = 0;
};

class lambertian : public material
{
public:

    explicit lambertian(color const& a) : m_albedo(a) {}

    auto get_color() const -> color override { return m_albedo; }

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

    metal(color const& a, double const fuzz) : m_albedo(a), m_fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

    auto get_color() const -> color override { return m_albedo; }

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

class dielectric : public material
{
public:

    explicit dielectric(double const index_of_refraction) : m_ir(index_of_refraction) {}

    auto get_color() const -> color override { return {1.0, 1.0, 1.0}; }

    auto scatter(ray const& r_in, hit_record const& rec, color& attenuation, ray& scattered) const -> bool override
    {
        attenuation = {1.0, 1.0, 1.0};
        auto const refraction_ratio = rec.front_face ? (1.0 / m_ir) : m_ir;
        auto const unit_direction = unit_vector(r_in.direction());
        auto const cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        auto const sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        auto const cannot_refract = refraction_ratio * sin_theta > 1.0;
        auto const direction = cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()
                                   ? reflect(unit_direction, rec.normal)
                                   : refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction);

        return true;
    }

private:

    static auto reflectance(double const cosine, double const ref_idx) -> double
    {
        auto const r0 = pow((1.0 - ref_idx) / (1.0 + ref_idx), 2.0);

        return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
    }

    double m_ir;
};