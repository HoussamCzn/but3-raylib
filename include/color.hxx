#pragma once

#include "vec3.hxx" // vec3

#include <iostream> // std::ostream

using color = vec3;

/**
 * @brief Write the translated [0,255] value of each color component.
 *
 * @param out         The output stream.
 * @param pixel_color The color to write.
 */
inline void write_color(std::ostream& out, color pixel_color)
{
    out << static_cast<int>(255.999 * pixel_color.x()) << ' ' << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
