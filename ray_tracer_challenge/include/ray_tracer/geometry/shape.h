#pragma once
#include <limits>
#include <ray_tracer/geometry/intersection.h>
#include <ray_tracer/lighting/material.h>
#include <ray_tracer/math/matrix.h>
#include <ray_tracer/math/ray.h>

namespace ray_tracer::geometry
{

struct Intersection;

class Shape
{
  private:
    math::Matrix transform = math::identity(4);
    math::Matrix inverse_transform = math::inverse_gauss_jordan(transform);
    std::size_t transform_version;

  protected:
    Shape() : transform(math::identity(4)), transform_version(0), id(next_id++) {};
    virtual void local_intersect(const math::Ray &ray, std::vector<Intersection> &world_intersections) const = 0;
    virtual Intersection min_intersect(const math::Ray &t_ray) const = 0;

  public:
    std::size_t get_transform_version() const
    {
        return transform_version;
    }

    virtual void get_all_intersections(const math::Ray &ray, std::vector<Intersection> &world_intersections) const
    {
        math::Ray t_ray = math::transform_ray(ray, get_inverse_transform());
        local_intersect(t_ray, world_intersections);
    }

    virtual Intersection get_min_intersection(const math::Ray &ray) const
    {
        math::Ray t_ray = math::transform_ray(ray, get_inverse_transform());
        return min_intersect(t_ray);
    }

    const math::Matrix &get_transform() const
    {
        return transform;
    };

    const math::Matrix &get_inverse_transform() const
    {
        return inverse_transform;
    };

    virtual ~Shape()
    {
    }

    std::size_t id;
    static inline std::size_t next_id = 0;
    lighting::Material material;
    virtual math::Tuple normal_at(const math::Tuple &world_point) const = 0;
    void set_transform(const math::Matrix &mat)
    {
        transform = mat;
        inverse_transform = math::inverse_gauss_jordan(mat);
        ++transform_version;
    }
};

} // namespace ray_tracer::geometry