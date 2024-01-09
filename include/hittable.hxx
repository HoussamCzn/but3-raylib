#pragma once

#include "interval.hxx"
#include "ray.hxx" // ray

class hit_record
{
public:

    auto set_face_normal(ray const& r, vec3 const& outward_normal) -> void
    {
        front_face = dot(r.direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }

    point3 p;
    vec3 normal;
    double t{0.0};
    bool front_face{false};
};

class hittable
{
public:

    virtual ~hittable() = default;

    virtual auto hit(ray const& r, interval ray_interval, hit_record& rec) const -> bool = 0;
};
