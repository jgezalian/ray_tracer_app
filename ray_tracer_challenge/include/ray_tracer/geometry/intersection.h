#pragma once
#include <vector>
#include <ray_tracer/math/ray.h>
#include <initializer_list>


namespace ray_tracer::geometry {

class Shape;

struct Intersection {
    double t;
    const Shape* shape;

    Intersection(double t_, const Shape* shape_) : t(t_), shape(shape_) {}
};


const Intersection* hit(const std::vector<Intersection> &inters);
}  // namespace ray_tracer::geometry
