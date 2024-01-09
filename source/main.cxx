#include "camera.hxx"        // camera
#include "hittable_list.hxx" // hittable_list
#include "material.hxx"      // lambertian, metal
#include "sphere.hxx"        // sphere

auto main() -> int
{
    auto const material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto const material_center = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto const material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto const material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    hittable_list world;
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera view;
    view.aspect_ratio = 16.0 / 9.0;
    view.image_width = 400;
    view.samples_per_pixel = 100;
    view.max_depth = 50;
    view.render(world);
}