#pragma once

#include "color.hxx"    // color
#include "hittable.hxx" // hittable

#include <iostream> // std::clog, std::cout, std::flush

class camera
{
public:

    auto aspect_ratio() const noexcept -> double { return m_aspect_ratio; }

    auto image_width() const noexcept -> int { return m_image_width; }

    auto set_aspect_ratio(double const aspect_ratio) noexcept -> void { m_aspect_ratio = aspect_ratio; }

    auto set_image_width(int const image_width) noexcept -> void { m_image_width = image_width; }

    auto render(hittable const& world) -> void
    {
        initialize();

        std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

        for (auto j = 0; j < m_image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;

            for (auto i = 0; i < m_image_width; ++i)
            {
                auto const u = static_cast<double>(i) / (m_image_width - 1);
                auto const v = static_cast<double>(j) / (m_image_height - 1);
                ray const r(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
                auto const pixel_color = ray_color(r, world);

                write_color(std::cout, pixel_color);
            }
        }
    }

private:

    auto initialize() -> void
    {
        m_image_height = static_cast<int>(m_image_width / m_aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;

        m_origin = point3(0.0, 0.0, 0.0);

        // Determine viewport dimensions.
        auto const focal_length = 1.0;
        auto const viewport_height = 2.0;
        auto const viewport_width = viewport_height * (static_cast<double>(m_image_width) / m_image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        m_horizontal = vec3(viewport_width, 0.0, 0.0);
        m_vertical = vec3(0.0, -viewport_height, 0.0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        auto const pixel_delta_u = m_horizontal / m_image_width;
        auto const pixel_delta_v = m_vertical / m_image_height;

        // Calculate the location of the upper left pixel.
        auto const viewport_upper_left = m_origin - vec3(0.0, 0.0, focal_length) - m_horizontal / 2 - m_vertical / 2;
        m_lower_left_corner = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    static auto ray_color(ray const& r, hittable const& world) -> color
    {
        hit_record rec;

        if (world.hit(r, interval{0.0, infinity}, rec)) { return 0.5 * (rec.normal + color{1.0, 1.0, 1.0}); }

        auto const unit_direction = unit_vector(r.direction());
        auto const t = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
    }

    double m_aspect_ratio{1.0};
    int m_image_width{100};
    int m_image_height{0};
    point3 m_origin{0.0, 0.0, 0.0};
    point3 m_lower_left_corner{0.0, 0.0, 0.0};
    vec3 m_horizontal{0.0, 0.0, 0.0};
    vec3 m_vertical{0.0, 0.0, 0.0};
};
