#include <ray_tracer/geometry/sphere.h>

namespace ray_tracer::geometry
{

using lighting::Material;
using math::inverse_gauss_jordan;
using math::norm;
using math::point;
using math::Ray;
using math::Tuple;

Tuple Sphere::normal_at(const Tuple &world_point) const
{
    const Tuple object_point(get_inverse_transform() * world_point);
    const Tuple object_normal(object_point - point(0, 0, 0));
    Tuple world_normal((get_inverse_transform().transpose()) * object_normal);
    world_normal.w = 0;
    return norm(world_normal);
}

std::pair<double, double> ray_sphere_intersect(const math::Ray &ray)
{
    Tuple shape_to_ray{ray.origin - point(0, 0, 0)};
    double a = dot(ray.direction, ray.direction);
    double b = 2 * dot(ray.direction, shape_to_ray);
    double c = dot(shape_to_ray, shape_to_ray) - 1;
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0)
        return {NAN, NAN};
    double t1 = (-1 * b - std::sqrt(discriminant)) / (2 * a);
    double t2 = (-1 * b + std::sqrt(discriminant)) / (2 * a);
    return {t1, t2};
}

void Sphere::local_intersect(const math::Ray &ray, std::vector<Intersection> &world_intersections) const
{
    std::pair<double, double> t_pair = ray_sphere_intersect(ray);
    if (std::isnan(t_pair.first) && std::isnan(t_pair.second))
        return;
    world_intersections.emplace_back(t_pair.first, this);
    world_intersections.emplace_back(t_pair.second, this);
}

Intersection Sphere::min_intersect(const math::Ray &t_ray) const
{
    std::pair<double, double> t_pair = ray_sphere_intersect(t_ray);
    if (std::isnan(t_pair.first) && std::isnan(t_pair.second))
        return Intersection(__DBL_MAX__, this);
    return t_pair.first > 0 ? Intersection(t_pair.first, this) : Intersection(t_pair.second, this);
}

} // namespace ray_tracer::geometry