#pragma once
#include <ray_tracer/physics/physics.h>

namespace ray_tracer::geometry
{
class Shape;
}

namespace ray_tracer::collision
{
struct HalfSpaceXZ;
struct Floor;
} // namespace ray_tracer::collision

namespace ray_tracer::world
{
struct World;
}

namespace ray_tracer::img
{
struct Canvas;
}

namespace ray_tracer::camera
{
struct Camera;
}

namespace ray_tracer::math
{
struct Matrix;
}

namespace ray_tracer::animation
{

struct Animation
{
    Animation()
    {
    }

    double x_left_bound = 100;
    double x_right_bound = 100;
    double y_top_bound = 100;
    double y_bottom_bound = 100;
    double z_near_bound = 100;
    double z_far_bound = 100;
    std::size_t max_anim_time = 30;
    double time_step = 0.00833333; // 1/120
    std::size_t fps = 120;

    void set_bounds()
    {
    }

    void animate_shape_raw_rgb(math::Matrix m, geometry::Shape *shape, physics::MotionState &state,
                               physics::MotionParams &params, world::World &world, camera::Camera &camera,
                               collision::HalfSpaceXZ right, collision::HalfSpaceXZ left, collision::Floor floor);

    void animate_shape_ppm(math::Matrix m, geometry::Shape *shape, physics::MotionState &state,
                           physics::MotionParams &params, world::World &world, camera::Camera &camera,
                           collision::HalfSpaceXZ right, collision::HalfSpaceXZ left, collision::Floor floor);
};
} // namespace ray_tracer::animation