#include <ray_tracer/camera/camera.h>
#include <ray_tracer/img/canvas.h>
#include <ray_tracer/math/ray.h>
#include <ray_tracer/math/util.h>
#include <ray_tracer/world/world.h>

#include <cassert>
#include <iostream>
#include <vector>

namespace ray_tracer::camera {

using geometry::Intersection;
using img::Canvas;
using img::Color;
using math::color_to_uint8;
using math::inverse_gauss_jordan;
using math::norm;
using math::point;
using math::print_tuple;
using math::Ray;
using math::Tuple;
using world::color_at;
using world::World;

[[maybe_unused]] constexpr double pi = 3.14159265358979323846;

Camera::Camera(std::size_t hsize_, std::size_t vsize_, double field_of_view_) :
    hsize(hsize_),
    vsize(vsize_),
    field_of_view(field_of_view_),
    aspect(static_cast<double>(hsize_) / static_cast<double>(vsize_)),
    half_view(std::tan(field_of_view_ / 2.0)),
    half_width(aspect >= 1.0 ? half_view : half_view * aspect),
    half_height(aspect >= 1.0 ? half_view / aspect : half_view),
    pixel_size((half_width * 2) / hsize) {
    assert(field_of_view_ > 0 && field_of_view_ < pi);
}

math::Ray Camera::ray_for_pixel(const std::size_t x, const std::size_t y) const {
    // std::vector<double> params = camera_params(hsize, vsize, field_of_view);
    double xoffset = (x + 0.5) * pixel_size;
    double yoffset = (y + 0.5) * pixel_size;
    double world_x = half_width - xoffset;
    double world_y = half_height - yoffset;
    Tuple pixel = get_inverse_transform() * point(world_x, world_y, -1);
    Tuple origin = get_inverse_transform() * point(0, 0, 0);
    Tuple direction = norm(pixel - origin);
    return Ray{origin, direction};
}

Canvas render(const Camera &camera, World &world) {
    Canvas image{camera.hsize, camera.vsize};
    std::size_t y;
    std::size_t x;
    world.cache_patterns();
    const World& const_world = world;
#pragma omp parallel for default(none) shared(image, camera, const_world) private(y, x)
    for (y = 0; y <= camera.vsize - 1; ++y) {
        for (x = 0; x <= camera.hsize - 1; ++x) {
            Ray ray{camera.ray_for_pixel(x, y)};
            Color color{color_at(const_world, ray)};
            image.write_pixel(x, y, color);
        }
    }
    return image;
}

void binary_out(const Camera &camera, World &world, std::ofstream &out) {
    std::size_t y;
    std::size_t x;
    std::vector<char> frame(camera.hsize * camera.vsize * 3);
    world.cache_patterns();
    const World& const_world = world;
    #pragma omp parallel for default(none) \
    shared(camera, const_world, frame) private(y, x)
    for (y = 0; y < camera.vsize; ++y) {
        for (x = 0; x < camera.hsize; ++x) {
            std::size_t idx = ((y * camera.hsize) + x) * 3;
            Ray ray{camera.ray_for_pixel(x, y)};
            Color color{color_at(const_world, ray)};
            frame[idx] = static_cast<char>(color_to_uint8(color.r));
            frame[idx + 1] = static_cast<char>(color_to_uint8(color.g));
            frame[idx + 2] = static_cast<char>(color_to_uint8(color.b));
        }
    }
    out.write(frame.data(), static_cast<std::streamsize>(frame.size()));
}

}  // namespace ray_tracer::camera