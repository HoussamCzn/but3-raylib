#pragma once

#include "hittable.hxx" // hittable
#include "ray.hxx"      // ray
#include "vec3.hxx"     // vec3

class triangle : public hittable
{
public:

    triangle(vec3 const& v0, vec3 const& v1, vec3 const& v2, std::shared_ptr<material> material_ptr)
        : m_v0(v0), m_v1(v1), m_v2(v2), m_material_ptr(std::move(material_ptr))
    {}

    auto hit(ray const& r, interval ray_interval, hit_record& rec) const -> bool override
    {
        auto const epsilon = 0.0000001;

        auto const edge1 = m_v1 - m_v0;
        auto const edge2 = m_v2 - m_v0;
        auto const h = cross(r.direction(), edge2);
        auto const a = dot(edge1, h);

        if (a > -epsilon && a < epsilon) { return false; }

        auto const f = 1.0 / a;
        auto const s = r.origin() - m_v0;
        auto const u = f * dot(s, h);

        if (u < 0.0 || u > 1.0) { return false; }

        auto const q = cross(s, edge1);
        auto const v = f * dot(r.direction(), q);

        if (v < 0.0 || u + v > 1.0) { return false; }

        auto const t = f * dot(edge2, q);

        if (!ray_interval.contains(t)) { return false; }

        rec.t = t;
        rec.p = r.at(rec.t);
        auto const outward_normal = cross(edge1, edge2);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = m_material_ptr;

        return true;
    }

private:

    vec3 m_v0;
    vec3 m_v1;
    vec3 m_v2;
    std::shared_ptr<material> m_material_ptr;
};
