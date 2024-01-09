#pragma once

#include "interval.hxx" // interval
#include "vec3.hxx"     // vec3

#include <algorithm> // std::ranges::transform
#include <cstdint>   // uint32_t
#include <iostream>  // std::ostream

using color = vec3;

/**
 * @brief Write the translated [0,255] value of each color component.
 *
 * @param out               The output stream.
 * @param pixel_color       The color to write.
 * @param samples_per_pixel The number of samples per pixel.
 */
inline auto write_color(std::ostream& out, color pixel_color, uint32_t samples_per_pixel) -> void
{
    static constexpr interval intensity(0.000, 0.999);
    auto const scale = 1.0 / samples_per_pixel;
    std::ranges::transform(pixel_color.e, pixel_color.e.begin(), [scale](auto const component) { return component * scale; });
    auto const [r, g, b] = pixel_color.e;

    out << static_cast<uint32_t>(256 * intensity.clamp(r)) << ' ' << static_cast<uint32_t>(256 * intensity.clamp(g)) << ' '
        << static_cast<uint32_t>(256 * intensity.clamp(b)) << '\n';
}