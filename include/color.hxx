#pragma once

#include "helpers.hxx"  // linear_to_gamma
#include "interval.hxx" // interval
#include "vec3.hxx"     // vec3

#include <algorithm> // std::ranges::transform
#include <cstdint>   // uint32_t
#include <format>    // std::format
#include <iostream>  // std::ostream
#include <string>    // std::string

using color = vec3;

/**
 * @brief Write the translated [0,255] value of each color component.
 * @param out               The output stream.
 * @param pixel_color       The color to write.
 * @param samples_per_pixel The number of samples per pixel.
 */
inline auto write_color(std::ostream& out, color pixel_color, uint32_t samples_per_pixel) -> void
{
    static constexpr interval intensity(0.000, 0.999);
    auto const correct = [scale = 1.0 / samples_per_pixel](auto const c) -> double { return linear_to_gamma(c * scale); };
    std::ranges::transform(pixel_color.e, pixel_color.e.begin(), correct);
    auto const [r, g, b] = pixel_color.e;
    auto const r_clamped = static_cast<uint32_t>(256 * intensity.clamp(r));
    auto const g_clamped = static_cast<uint32_t>(256 * intensity.clamp(g));
    auto const b_clamped = static_cast<uint32_t>(256 * intensity.clamp(b));
    out << std::format("{} {} {}\n", r_clamped, g_clamped, b_clamped);
}

/**
 * @brief Write the translated [0,255] value of each color component.
 * @param buffer            The output buffer to append to.
 * @param pixel_color       The color to write.
 * @param samples_per_pixel The number of samples per pixel.
 */
inline auto write_color(std::string& buffer, color pixel_color, uint32_t samples_per_pixel) -> void
{
    static constexpr interval intensity(0.000, 0.999);
    auto const correct = [scale = 1.0 / samples_per_pixel](auto const c) -> double { return linear_to_gamma(c * scale); };
    std::ranges::transform(pixel_color.e, pixel_color.e.begin(), correct);
    auto const [r, g, b] = pixel_color.e;
    auto const r_clamped = static_cast<uint32_t>(256 * intensity.clamp(r));
    auto const g_clamped = static_cast<uint32_t>(256 * intensity.clamp(g));
    auto const b_clamped = static_cast<uint32_t>(256 * intensity.clamp(b));
    buffer += std::format("{} {} {}\n", r_clamped, g_clamped, b_clamped);
}

/**
 * @brief Create a color object from the given RGB values.
 * @note  The RGB values are expected to be in the range [0,255].
 *
 * @param r The red component.
 * @param g The green component.
 * @param b The blue component.
 * @return The color object.
 */
constexpr auto color_from_rgb(uint8_t r, uint8_t g, uint8_t b) -> color { return {r / 255.0, g / 255.0, b / 255.0}; }