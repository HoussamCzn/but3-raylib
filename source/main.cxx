#include "color.hxx"         // color, write_color
#include "constants.hxx"     // infinity, pi
#include "hittable.hxx"      // hittable
#include "hittable_list.hxx" // hittable_list
#include "sphere.hxx"        // sphere

#include <iostream> // std::cout, std::clog, std::flush

auto ray_color(ray const& r, hittable const& world) -> color
{
    hit_record rec;
    
    if (world.hit(r, interval(0, infinity), rec)) { return 0.5 * (rec.normal + color(1, 1, 1)); }

    vec3 const unit_direction = unit_vector(r.direction());
    auto const a = 0.5 * (unit_direction.y() + 1.0);
    
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

auto main() -> int
{
    // Image
    auto const aspect_ratio = 16.0 / 9.0;
    auto const image_width = 400;
    auto const image_height = static_cast<int>(image_width / aspect_ratio) < 1 ? 1 : static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    auto const viewport_height = 2.0;
    auto const viewport_width = aspect_ratio * viewport_height;
    auto const focal_length = 1.0;
    auto const camera_center = point3(0, 0, 0);

    // Compute the vectors across the horizontal and vertical axes of the viewport.
    auto const viewport_u = vec3(viewport_width, 0, 0);
    auto const viewport_v = vec3(0, viewport_height, 0);

    // Compute the lower left corner of the viewport.
    auto const pixel_delta_x = viewport_u / image_width;
    auto const pixel_delta_y = viewport_v / image_height;

    // Compute the location of the upper left corner of the viewport.
    auto const viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto const pixel00_location = viewport_upper_left + pixel_delta_x / 2 + pixel_delta_y / 2;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (auto j = image_height - 1; j >= 0; --j)
    {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (auto i = 0; i < image_width; ++i)
        {
            auto const u = static_cast<double>(i) / (image_width - 1);
            auto const v = static_cast<double>(j) / (image_height - 1);
            auto const pixel_location = viewport_upper_left + u * viewport_u + v * viewport_v;
            auto const r = ray(camera_center, pixel_location - camera_center);
            auto const pixel_color = ray_color(r, world);

            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
