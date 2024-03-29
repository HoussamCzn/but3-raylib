#pragma once

#include "color.hxx"    // color
#include "helpers.hxx"  // random_double
#include "hittable.hxx" // hittable
#include "light.hxx"    // light
#include "material.hxx" // material
#include "vec3.hxx"     // vec3

#include <cstdint>    // uint32_t
#include <format>     // std::format
#include <iostream>   // std::clog, std::cout, std::flush
#include <latch>      // std::latch
#include <numeric>    // std::transform_reduce
#include <ranges>     // std::views::iota
#include <syncstream> // std::osyncstream
#include <thread>     // std::thread

enum class render_mode
{
    normals,
    final
};

class camera
{
public:

    double aspect_ratio{1.0};
    uint32_t image_width{100};
    uint32_t samples_per_pixel{10};
    uint32_t max_depth{10};

    auto render(hittable const& world, std::span<light const> lights = {}, render_mode const mode = render_mode::final,
                size_t const thread_count = std::thread::hardware_concurrency()) -> void
    {
        initialize();
        std::cout << std::format("P3\n{} {}\n255\n", image_width, m_image_height);

        switch (thread_count)
        {
        case 0:
        case 1:
            singlethreaded_render(world, lights, mode);
            break;
        default:
            multithreaded_render(world, lights, mode, thread_count);
            break;
        }
    }

private:

    constexpr auto initialize() -> void
    {
        m_image_height = static_cast<int>(image_width / aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;

        m_origin = point3(0.0, 0.0, 5.0);

        // Determine viewport dimensions.
        auto const focal_length = 5.0;
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

    static auto is_shadowed(vec3 const& point, vec3 const& to_light, double light_distance, hittable const& world) -> bool
    {
        ray shadow_ray(point, to_light);
        hit_record shadow_rec;

        return world.hit(shadow_ray, {0.001, light_distance}, shadow_rec);
    }

    static auto ray_color(ray const& r, uint32_t depth, hittable const& world, std::span<light const> lights,
                          render_mode const mode) -> color
    {
        if (depth <= 0) [[unlikely]] { return {}; }

        hit_record rec;

        if (world.hit(r, interval{0.001, infinity}, rec))
        {
            color diffuse_light;

            for (auto const& source : lights)
            {
                auto const to_light = source.position - rec.p;
                auto const light_distance = to_light.length();
                auto const light_direction = to_light / light_distance;

                if (!is_shadowed(rec.p, light_direction, light_distance, world))
                {
                    auto const light_intensity = source.intensity / (light_distance * light_distance);
                    auto const light_attenuation = std::max(0.0, dot(rec.normal, light_direction));
                    diffuse_light += light_attenuation * source.color * light_intensity * rec.mat_ptr->get_color();
                }
            }

            if (mode == render_mode::normals) { return 0.5 * (rec.normal + color(1.0, 1.0, 1.0)); }

            return diffuse_light;
        }

        auto const unit_direction = unit_vector(r.direction());
        auto const t = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
    }

    auto singlethreaded_render(hittable const& world, std::span<light const> lights, render_mode const mode) -> void
    {
        for (auto const j : std::views::iota(0U, m_image_height))
        {
            std::clog << "\rScanlines remaining: " << m_image_height - j << ' ' << std::flush;

            for (auto const i : std::views::iota(0U, image_width))
            {
                auto const sp_range = std::views::iota(0U, samples_per_pixel);
                auto const render = [this, &world, &lights, mode, i, j](auto const) {
                    return ray_color(get_ray(i, j), max_depth, world, lights, mode);
                };
                auto const px_color = std::transform_reduce(sp_range.begin(), sp_range.end(), color{}, std::plus{}, render);
                write_color(std::cout, px_color, samples_per_pixel);
            }
        }

        std::clog << "\nDone.\n";
    }

    auto multithreaded_render(hittable const& world, std::span<light const> lights, render_mode const mode,
                              size_t const thread_count) -> void
    {
        std::vector<std::thread> threads(thread_count);
        std::vector<std::string> buffers(thread_count);
        std::latch done(static_cast<ptrdiff_t>(thread_count));
        std::size_t remaining_lines(m_image_height);
        auto const rows = m_image_height / thread_count;
        auto const work = [this, &world, &lights, &buffers, &done, &remaining_lines, mode](auto idx, auto start, auto end) {
            auto& buffer = buffers[idx];
            buffer.reserve((end - start) * image_width * 12);

            for (auto const j : std::views::iota(start, end))
            {
                std::osyncstream osync(std::clog);
                osync << "\rScanlines remaining: " << --remaining_lines << ' ' << std::flush;

                for (auto const i : std::views::iota(0U, image_width))
                {
                    auto const sp_range = std::views::iota(0U, samples_per_pixel);
                    auto const render = [this, i, j, &world, &lights, mode](auto const) {
                        return ray_color(get_ray(i, j), max_depth, world, lights, mode);
                    };
                    auto const px_color = std::transform_reduce(sp_range.begin(), sp_range.end(), color{}, std::plus{}, render);
                    write_color(buffer, px_color, samples_per_pixel);
                }
            }

            done.count_down();
        };

        for (auto const i : std::views::iota(0U, thread_count))
        {
            auto const start = i * rows;
            auto const end = (i == thread_count - 1) ? m_image_height : start + rows;
            threads.emplace_back(work, i, start, end);
        }

        done.wait();
        std::ranges::for_each(threads, [](auto& thread) {
            if (thread.joinable()) [[likely]] { thread.join(); }
        });
        std::ranges::for_each(buffers, [](auto const& buffer) { std::cout << buffer; });
        std::clog << "\nDone.\n";
    }

    uint32_t m_image_height{0};
    point3 m_origin;
    point3 m_lower_left_corner;
    vec3 m_horizontal;
    vec3 m_vertical;
};
