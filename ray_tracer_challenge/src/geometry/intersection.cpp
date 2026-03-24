#include <ray_tracer/geometry/intersection.h>
// #include <ray_tracer/geometry/sphere.h>
#include <ray_tracer/geometry/shape.h>
#include <ray_tracer/math/ray.h>

namespace ray_tracer::geometry {

using math::inverse_gauss_jordan;
using math::point;
using math::print_matrix;
using math::Ray;
using math::transform_ray;
using math::Tuple;


const Intersection *hit(const std::vector<Intersection> &inters) {
    const Intersection *hit = nullptr;
    double min_t = std::numeric_limits<double>::infinity();
    for (auto &inter : inters) {
        if (inter.t >= 0 && inter.t < min_t) {
            min_t = inter.t;
            hit = &inter;
        }
    }
    return hit;
}

}  // namespace ray_tracer::geometry