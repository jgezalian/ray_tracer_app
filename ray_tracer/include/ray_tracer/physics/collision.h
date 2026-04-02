#pragma once
#include <ray_tracer/math/tuple.h>

namespace ray_tracer::physics
{
struct MotionState;
}

namespace ray_tracer::collision
{

struct Floor
{
    double y = 0;
};

struct LineXZ
{
    math::Tuple point = math::point(0, 0, 0);
    math::Tuple direction = math::vector(0, 0, 0);
};

struct HalfSpaceXZ
{
    LineXZ boundary;
    math::Tuple inward_normal = math::vector(0, 0, 0);
};



struct CrossingResult
{
    bool crossed = false;
};

struct FloorBounds
{
    Floor floor;
    HalfSpaceXZ left;
    HalfSpaceXZ right;
};

double signed_distance_from_xz_boundary(const math::Tuple &p, const HalfSpaceXZ &half_space);
bool sphere_crosses_boundary_xz(const physics::MotionState &a, const physics::MotionState &b, const double radius,
                                const HalfSpaceXZ &half_space);
bool sphere_crosses_floor(const Floor floor, const physics::MotionState &a, const physics::MotionState &b, const double radius);



} // namespace ray_tracer::collision