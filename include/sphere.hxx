#pragma once

#include "hittable.hxx" // hittable
#include "vec3.hxx"     // vec3

class sphere : public hittable
{
public:

    sphere(point3 const& center, double radius) : m_center(center), m_radius(radius) {}

    auto hit(ray const& r, double t_min, double t_max, hit_record& rec) const -> bool override
    {
        vec3 const oc = r.origin() - m_center;
        auto const a = r.direction().length_squared();
        auto const half_b = dot(oc, r.direction());
        auto const c = oc.length_squared() - m_radius * m_radius;
        auto const discriminant = half_b * half_b - a * c;

        if (discriminant > 0.0)
        {
            auto const root = sqrt(discriminant);
            auto temp = (-half_b - root) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.at(rec.t);
                vec3 const outward_normal = (rec.p - m_center) / m_radius;
                rec.set_face_normal(r, outward_normal);
                
                return true;
            }
            
            temp = (-half_b + root) / a;
            
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.at(rec.t);
                vec3 const outward_normal = (rec.p - m_center) / m_radius;
                rec.set_face_normal(r, outward_normal);
                
                return true;
            }
        }
        
        return false;
    }

private:

    point3 m_center;
    double m_radius;
};
