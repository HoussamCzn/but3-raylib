#include "color.hxx" // color
#include "vec3.hxx"  // vec3

#include <iostream> // std::cout, std::clog, std::flush

auto main() -> int
{
    // Image
    int image_width = 256;
    int image_height = 256;
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            auto const pixel_color =
                color{static_cast<double>(i) / (image_width - 1), static_cast<double>(j) / (image_height - 1), 0};
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}