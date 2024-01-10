#pragma once

#include "ray.hxx" // point3

#include <memory> // std::shared_ptr

class material;

class hit_record
{
public:

    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    double t{0.0};
    bool front_face{false};

    constexpr auto set_face_normal(ray const& r, vec3 const& outward_normal) -> void
    {
        front_face = dot(r.direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};