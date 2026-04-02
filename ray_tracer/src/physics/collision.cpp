#include <ray_tracer/physics/collision.h>
#include <ray_tracer/physics/physics.h>
using ray_tracer::collision::HalfSpaceXZ;
using ray_tracer::math::dot;
using ray_tracer::math::Tuple;
using ray_tracer::math::vector;
using ray_tracer::physics::advance;

namespace ray_tracer::collision
{

double signed_distance_from_xz_boundary(const Tuple &p, const HalfSpaceXZ &half_space)
{
    Tuple shape_to_p = p - half_space.boundary.point;
    Tuple shape_to_p_vec = vector(shape_to_p.x, shape_to_p.y, shape_to_p.z);

    return dot(shape_to_p_vec, half_space.inward_normal);
}

bool sphere_crosses_boundary_xz(const physics::MotionState &a, const physics::MotionState &b, const double radius,
                                const HalfSpaceXZ &half_space)
{
    double signed_dist_a = signed_distance_from_xz_boundary(a.pos, half_space);
    double signed_dist_b = signed_distance_from_xz_boundary(b.pos, half_space);

    double adjusted_a = signed_dist_a - radius;
    double adjusted_b = signed_dist_b - radius;

    if (adjusted_a > 0 && adjusted_b <= 0)
        return true;
    else
        return false;
}

bool sphere_crosses_floor(const Floor floor, const physics::MotionState &a, const physics::MotionState &b,
                          const double radius)
{
    double edge_at_a = (a.pos.y - radius);
    double edge_at_b = (b.pos.y - radius);
    if (edge_at_a >= floor.y && edge_at_b < floor.y)
        return true;
    else
        return false;
}


} // namespace ray_tracer::collision