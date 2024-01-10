#include "camera.hxx"        // camera
#include "hittable_list.hxx" // hittable_list
#include "material.hxx"      // lambertian, metal, dialectric
#include "triangle.hxx"      // triangle

auto main() -> int
{
    auto const lambertian_material = std::make_shared<lambertian>(color_from_rgb(255, 0, 0));
    vec3 const v0(0, 0, -1);
    vec3 const v1(0.6, 0, -1);
    vec3 const v2(0, 0.4, -1);

    hittable_list world;
    world.add(std::make_shared<triangle>(v0, v1, v2, lambertian_material));

    camera view;
    view.aspect_ratio = 16.0 / 9.0;
    view.image_width = 400;
    view.samples_per_pixel = 100;
    view.max_depth = 50;
    view.render(world);

    return 0;
}