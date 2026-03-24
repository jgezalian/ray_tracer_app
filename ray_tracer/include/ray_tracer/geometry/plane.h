#pragma once
#include <ray_tracer/geometry/shape.h>


namespace ray_tracer::geometry {

struct Plane : public Shape{
    Plane() : Shape() {}
    math::Tuple normal_at(const math::Tuple &world_point) const override;
    void local_intersect(const math::Ray &ray, std::vector<Intersection>& world_intersections) const override;
    Intersection min_intersect(const math::Ray &t_ray) const override;
    
};


}  // namespace ray_tracer::geometry