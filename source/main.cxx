#include "camera.hxx"        // camera
#include "hittable_list.hxx" // hittable_list
#include "sphere.hxx"        // sphere

auto main() -> int
{
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera view;
    view.aspect_ratio = 16.0 / 9.0;
    view.image_width = 400;
    view.samples_per_pixel = 100;
    view.max_depth = 50;
    view.render(world);
}