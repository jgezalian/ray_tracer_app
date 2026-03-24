#pragma once
#include <ray_tracer/geometry/intersection.h>
#include <ray_tracer/geometry/shape.h>
#include <ray_tracer/world/world.h>

namespace ray_tracer::world {
struct World;
}

namespace ray_tracer::helpers {

struct Computation {
    double t = 0;
    const geometry::Shape *shape = nullptr;
    math::Tuple point = math::point(0, 0, 0);
    math::Tuple over_point = math::point(0, 0, 0);
    math::Tuple eyev = math::vector(0, 0, 0);
    math::Tuple normalv = math::vector(0, 0, 0);
    bool inside = false;
};

Computation prepare_computation(const ray_tracer::geometry::Intersection &i,
                                const ray_tracer::math::Ray &r);

}  // namespace ray_tracer::helpers