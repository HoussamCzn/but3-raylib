#include "camera.hxx"        // camera
#include "hittable_list.hxx" // hittable_list
#include "material.hxx"      // lambertian, metal
#include "sphere.hxx"        // sphere
#include "triangle.hxx"
#include "vec3.hxx"

auto main() -> int
{
    auto const metal_material = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    hittable_list world;
    world.add(make_shared<triangle>(vec3(0, 0, -1), vec3(0.6, 0, -1), vec3(0, 0.4, -1), metal_material));

    camera view;
    view.aspect_ratio = 16.0 / 9.0;
    view.image_width = 400;
    view.samples_per_pixel = 100;
    view.max_depth = 50;
    view.render(world);
}