#pragma once

#include "color.hxx"    // color
#include "helpers.hxx"  // random_double
#include "hittable.hxx" // hittable
#include "vec3.hxx"     // vec3

#include <cstdint>  // uint32_t
#include <iostream> // std::clog, std::cout, std::flush

class camera
{
public:

    double aspect_ratio{1.0};
    uint32_t image_width{100};
    uint32_t samples_per_pixel{10};
    uint32_t max_depth{10};

    auto render(hittable const& world) -> void
    {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << m_image_height << "\n255\n";

        for (auto j = 0U; j < m_image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;

            for (auto i = 0U; i < image_width; ++i)
            {
                color pixel_color;

                for (auto s = 0U; s < samples_per_pixel; ++s)
                {
                    auto const r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\nDone.\n";
    }

private:

    constexpr auto initialize() -> void
    {
        m_image_height = static_cast<int>(image_width / aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;

        m_origin = point3(0.0, 0.0, 0.0);

        // Determine viewport dimensions.
        auto const focal_length = 1.0;
        auto const viewport_height = 2.0;
        auto const viewport_width = viewport_height * (static_cast<double>(image_width) / m_image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        m_horizontal = vec3(viewport_width, 0.0, 0.0);
        m_vertical = vec3(0.0, -viewport_height, 0.0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        auto const pixel_delta_u = m_horizontal / image_width;
        auto const pixel_delta_v = m_vertical / m_image_height;

        // Calculate the location of the upper left pixel.
        auto const viewport_upper_left = m_origin - vec3(0.0, 0.0, focal_length) - m_horizontal / 2 - m_vertical / 2;
        m_lower_left_corner = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    auto get_ray(uint32_t i, uint32_t j) const -> ray
    {
        auto const pixel_offset = pixel_sample_square();
        auto const horizontal_offset = (i + pixel_offset.x()) / (image_width - 1);
        auto const vertical_offset = (j + pixel_offset.y()) / (m_image_height - 1);
        auto const direction = m_lower_left_corner + horizontal_offset * m_horizontal + vertical_offset * m_vertical - m_origin;

        return {m_origin, direction};
    }

    auto pixel_sample_square() const -> vec3
    {
        auto const x = -0.5 + random_double();
        auto const y = -0.5 + random_double();

        return (x * m_horizontal) + (y * m_vertical);
    }

    static auto ray_color(ray const& r, uint32_t depth, hittable const& world) -> color
    {
        if (depth <= 0) { return {}; }

        hit_record rec;

        if (world.hit(r, interval{0.001, infinity}, rec))
        {
            auto const direction = rec.normal + random_unit_vector();

            return 0.5 * ray_color({rec.p, direction}, depth - 1, world);
        }

        auto const unit_direction = unit_vector(r.direction());
        auto const t = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
    }

    uint32_t m_image_height{0};
    point3 m_origin;
    point3 m_lower_left_corner;
    vec3 m_horizontal;
    vec3 m_vertical;
};
