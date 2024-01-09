#include "color.hxx" // color
#include "ray.hxx"   // point3
#include "vec3.hxx"  // vec3, dot, unit_vector

#include <iostream> // std::cout, std::clog, std::flush

/**
 * @brief Determines if a ray hits a sphere.
 * The current implementation only works for spheres centered at the origin.
 * @param center The center of the sphere.
 * @param radius The radius of the sphere.
 * @param r      The ray.
 * @return The distance along the ray at which it hits the sphere, or -1.0 if it does not hit.
 */
auto hit_sphere(point3 const& center, double radius, ray const& r) -> double
{
    vec3 const oc = r.origin() - center;
    auto const a = dot(r.direction(), r.direction());
    auto const b = 2.0 * dot(oc, r.direction());
    auto const c = dot(oc, oc) - radius * radius;
    auto const discriminant = b * b - 4 * a * c;

    if (discriminant < 0) { return -1.0; }
    return (-b - sqrt(discriminant)) / (2.0 * a);
}

/**
 * @brief Determines the color of a ray.
 * @param r The ray.
 * @return The color of the ray.
 */
auto sphere_ray_color(ray const& r) -> color
{
    auto const t = hit_sphere(point3(0, 0, -1), 0.5, r);

    if (t > 0.0)
    {
        auto const n = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(n.x() + 1, n.y() + 1, n.z() + 1);
    }

    vec3 const unit_direction = unit_vector(r.direction());
    auto const a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

/**
 * @brief Determines if a ray hits a triangle.
 * @param v0 The first vertex of the triangle.
 * @param v1 The second vertex of the triangle.
 * @param v2 The third vertex of the triangle.
 * @param r  The ray.
 * @return The distance along the ray at which it hits the triangle, or -1.0 if it does not hit.
 */
auto hit_triangle(vec3 const& v0, vec3 const& v1, vec3 const& v2, ray const& r) -> double
{
    auto const epsilon = 0.0000001;

    auto const edge1 = v1 - v0;
    auto const edge2 = v2 - v0;
    auto const h = cross(r.direction(), edge2);
    auto const a = dot(edge1, h);

    if (a > -epsilon && a < epsilon) { return -1.0; }

    auto const f = 1.0 / a;
    auto const s = r.origin() - v0;
    auto const u = f * dot(s, h);

    if (u < 0.0 || u > 1.0) { return -1.0; }

    auto const q = cross(s, edge1);
    auto const v = f * dot(r.direction(), q);

    if (v < 0.0 || u + v > 1.0) { return -1.0; }

    auto const t = f * dot(edge2, q);

    return t > epsilon ? t : -1.0;
}

/**
 * @brief Determines the color of a ray.
 * @param r The ray.
 * @return The color of the ray.
 */
auto triangle_ray_color(ray const& r) -> color
{
    // v0(0, 0, -1.3), v1(0.6, 0, -1), v2(0, 0.4, -1)
    vec3 const v0{0, 0, -1.3};
    vec3 const v1{0.6, 0, -1};
    vec3 const v2{0, 0.4, -1};
    auto const t = hit_triangle(v0, v1, v2, r);

    if (t > 0.0)
    {
        vec3 const normal = unit_vector(cross(v1 - v0, v2 - v0));
        return 0.5 * color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
    }

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

    // Camera
    auto const focal_length = 1.0;
    auto const viewport_height = 2.0;
    auto const viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto const camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto const viewport_u = vec3(viewport_width, 0, 0);
    auto const viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto const pixel_delta_u = viewport_u / image_width;
    auto const pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto const viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto const pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            auto const pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto const ray_direction = pixel_center - camera_center;
            ray const r(camera_center, ray_direction);
            auto const pixel_color = triangle_ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
