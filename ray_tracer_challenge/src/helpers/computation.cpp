#include <ray_tracer/helpers/computation.h>
#include <ray_tracer/geometry/intersection.h>
#include <ray_tracer/math/ray.h>

namespace ray_tracer::helpers {

    using geometry::Intersection;
    using math::Ray;
    using math::dot;
    using math::print_tuple;

    constexpr double eps = 1e-5;

    Computation prepare_computation(const Intersection &i, const Ray &r) {
        Computation comps;
        comps.t = i.t;
        comps.shape = i.shape;
        comps.point = r.position(comps.t);
        comps.eyev = -r.direction;
        comps.normalv = comps.shape->normal_at(comps.point);

        if(dot(comps.normalv, comps.eyev) < 0) {
            comps.inside = true;
            comps.normalv = -comps.normalv;
        }

        comps.over_point = comps.point + comps.normalv * eps;
        
        return comps;
    }

}