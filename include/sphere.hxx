#pragma once

#include "hittable.hxx" // hittable
#include "vec3.hxx"     // vec3

class sphere : public hittable
{
public:

    sphere(point3 center, double radius, std::shared_ptr<material> material_ptr)
        : m_center(center), m_radius(radius), m_material_ptr(std::move(material_ptr))
    {}

    auto hit(ray const& r, interval ray_interval, hit_record& rec) const -> bool override
    {
        auto const oc = r.origin() - m_center;
        auto const a = r.direction().length_squared();
        auto const half_b = dot(oc, r.direction());
        auto const c = oc.length_squared() - m_radius * m_radius;
        auto const discriminant = half_b * half_b - a * c;

        if (discriminant > 0.0)
        {
            auto const sqrtd = std::sqrt(discriminant);
            auto root = (-half_b - sqrtd) / a;

            if (!ray_interval.surrounds(root))
            {
                root = (-half_b + root) / a;

                if (!ray_interval.surrounds(root)) { return false; }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            auto const outward_normal = (rec.p - m_center) / m_radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = m_material_ptr;

            return true;
        }

        return false;
    }

private:

    point3 m_center;
    double m_radius;
    std::shared_ptr<material> m_material_ptr;
};
