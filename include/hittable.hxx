#pragma once

#include "interval.hxx" // interval
#include "ray.hxx"      // ray
#include "record.hxx"   // hit_record

class hittable
{
public:

    virtual ~hittable() = default;

    virtual auto hit(ray const& r, interval ray_interval, hit_record& rec) const -> bool = 0;
};
