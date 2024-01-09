#pragma once

#include "hittable.hxx" // hittable, hit_record

#include <memory> // std::make_shared
#include <vector> // std::vector

class hittable_list : public hittable
{
public:

    hittable_list() = default;

    explicit hittable_list(std::shared_ptr<hittable> object) { add(object); }

    auto clear() -> void { m_objects.clear(); }

    auto add(std::shared_ptr<hittable> object) -> void { m_objects.push_back(object); }

    auto hit(ray const& r, interval ray_interval, hit_record& rec) const -> bool override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_interval.max;

        for (auto const& object : m_objects)
        {
            if (object->hit(r, {ray_interval.min, closest_so_far}, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

private:

    std::vector<std::shared_ptr<hittable>> m_objects;
};