#include "camera.hxx"        // camera
#include "hittable_list.hxx" // hittable_list
#include "material.hxx"      // lambertian, metal, dialectric
#include "triangle.hxx"      // triangle

#include <algorithm>  // std::ranges::for_each
#include <charconv>   // std::from_chars
#include <filesystem> // std::filesystem::path
#include <fstream>    // std::ifstream
#include <ranges>     // std::views::iota
#include <stdexcept>  // std::runtime_error

auto load_mesh(std::filesystem::path const& path) -> hittable_list
{
    if (!std::filesystem::exists(path))
    {
        throw std::filesystem::filesystem_error("file not found", path,
                                                std::make_error_code(std::errc::no_such_file_or_directory));
    }

    std::ifstream file(path);
    if (!file.is_open()) [[unlikely]] { throw std::runtime_error("file not opened"); }

    hittable_list world;
    static constexpr std::ptrdiff_t vertex_count_offset{15L};
    static constexpr std::ptrdiff_t face_count_offset{13L};
    std::size_t vertex_count{0UL};
    std::size_t face_count{0UL};
    std::string line;

    while (std::getline(file, line))
    {
        if (line.starts_with("end_header")) { break; }

        auto const* start = line.data();
        auto const* end = line.data();
        std::ranges::advance(end, static_cast<std::ptrdiff_t>(line.size()));

        if (line.starts_with("element vertex"))
        {
            std::ranges::advance(start, vertex_count_offset);
            auto const [ptr, ec] = std::from_chars(start, end, vertex_count);

            if (ec != std::errc()) [[unlikely]] { throw std::runtime_error("invalid vertex count"); }
        }
        else if (line.starts_with("element face"))
        {
            std::ranges::advance(start, face_count_offset);
            auto const [ptr, ec] = std::from_chars(start, end, face_count);

            if (ec != std::errc()) [[unlikely]] { throw std::runtime_error("invalid face count"); }
        }
    }

    std::vector<vec3> vertices;
    vertices.reserve(vertex_count);
    std::ranges::for_each(std::views::iota(0UL, vertex_count), [&vertices, &file]([[maybe_unused]] std::size_t const idx) {
        float x{0.0F};
        float y{0.0F};
        float z{0.0F};
        file >> x >> y >> z;
        vertices.emplace_back(x, y, z);
    });

    std::ranges::for_each(std::views::iota(0UL, face_count), [&]([[maybe_unused]] std::size_t const idx) {
        std::size_t v1{0UL};
        std::size_t v2{0UL};
        std::size_t v3{0UL};
        file >> vertex_count >> v1 >> v2 >> v3;
        world.add(std::make_shared<triangle>(vertices[v1], vertices[v2], vertices[v3],
                                             std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3)));
    });

    return world;
}

auto main(int argc, char* argv[]) -> int
{
    std::span<char const* const> args(argv, argc);

    if (argc != 2)
    {
        std::cerr << "Usage: " << args[0] << " <input file>\n";
        return -1;
    }

    try
    {
        auto const world = load_mesh(args[1]);

        camera view;
        view.aspect_ratio = 16.0 / 9.0;
        view.image_width = 400;
        view.samples_per_pixel = 100;
        view.max_depth = 50;
        view.render(world);
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}