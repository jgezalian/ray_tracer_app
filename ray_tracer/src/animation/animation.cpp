#include <iostream>
#include <ray_tracer/animation/animation.h>
#include <ray_tracer/camera/camera.h>
#include <ray_tracer/geometry/shape.h>
#include <ray_tracer/img/canvas.h>
#include <ray_tracer/math/transform.h>
#include <ray_tracer/physics/collision.h>
#include <ray_tracer/world/world.h>
#include <sstream>

using ray_tracer::camera::binary_out;
using ray_tracer::camera::Camera;
using ray_tracer::camera::render;
using ray_tracer::collision::Floor;
using ray_tracer::collision::HalfSpaceXZ;
using ray_tracer::collision::signed_distance_from_xz_boundary;
using ray_tracer::collision::sphere_crosses_boundary_xz;
using ray_tracer::collision::sphere_crosses_floor;
using ray_tracer::img::Canvas;
using ray_tracer::math::chain_transform;
using ray_tracer::math::scaling;
using ray_tracer::math::translation;
using ray_tracer::math::Tuple;
using ray_tracer::physics::advance;
using ray_tracer::physics::MotionParams;
using ray_tracer::physics::MotionState;
using ray_tracer::world::World;

namespace ray_tracer::animation
{

double constexpr eps = 1e-5;

void Animation::animate_shape_raw_rgb(math::Matrix m, geometry::Shape *shape, physics::MotionState &state,
                                      physics::MotionParams &params, World &world, Camera &camera, HalfSpaceXZ right,
                                      HalfSpaceXZ left, Floor floor)
{
    std::ofstream out("render.rgb", std::ios::binary);
    for (std::size_t i = 0; i < max_anim_time * fps; ++i)
    {
        MotionState cur = advance(state, i * time_step, params);
        MotionState next = advance(state, (i + 1) * time_step, params);
        const double radius = 1.0;

        double right_a = signed_distance_from_xz_boundary(cur.pos, right) - radius;
        double right_b = signed_distance_from_xz_boundary(next.pos, right) - radius;

        if (right_a > 0 && right_b <= 0)
        {
            double alpha = right_a / (right_a - right_b);
            Tuple hit_pos = cur.pos + alpha * (next.pos - cur.pos);
            shape->set_transform(chain_transform({m, translation(hit_pos.x, hit_pos.y, hit_pos.z)}));
            binary_out(camera, world, out);
            break;
        }

        double left_a = signed_distance_from_xz_boundary(cur.pos, left) - radius;
        double left_b = signed_distance_from_xz_boundary(next.pos, left) - radius;

        if (left_a > 0 && left_b <= 0)
        {
            double alpha = left_a / (left_a - left_b);
            Tuple hit_pos = cur.pos + alpha * (next.pos - cur.pos);
            shape->set_transform(chain_transform({m, translation(hit_pos.x, hit_pos.y, hit_pos.z)}));
            binary_out(camera, world, out);
            break;
        }

        double floor_a = (cur.pos.y - radius) - floor.y;
        double floor_b = (next.pos.y - radius) - floor.y;
        if (floor_a >= -eps && floor_b <= eps)
        {
            double alpha = floor_a / (floor_a - floor_b);
            Tuple hit_pos = cur.pos + alpha * (next.pos - cur.pos);
            shape->set_transform(chain_transform({m, translation(hit_pos.x, hit_pos.y, hit_pos.z)}));
            binary_out(camera, world, out);
            break;
        }

        shape->set_transform(chain_transform({m, translation(cur.pos.x, cur.pos.y, cur.pos.z)}));
        binary_out(camera, world, out);
    }
}

void Animation::animate_shape_ppm(math::Matrix m, geometry::Shape *shape, physics::MotionState &state,
                                  physics::MotionParams &params, World &world, Camera &camera, HalfSpaceXZ right,
                                  HalfSpaceXZ left, Floor floor)
{
    for (std::size_t i = 0; i < max_anim_time * fps; ++i)
    {
        MotionState cur = advance(state, i * time_step, params);
        MotionState next = advance(state, (i + 1) * time_step, params);
        const double radius = 1.0;
        Tuple render_pos = cur.pos;
        bool collided = false;

        double right_a = signed_distance_from_xz_boundary(cur.pos, right) - radius;
        double right_b = signed_distance_from_xz_boundary(next.pos, right) - radius;
        if (right_a > 0 && right_b <= 0)
        {
            double alpha = right_a / (right_a - right_b);
            render_pos = cur.pos + alpha * (next.pos - cur.pos);
            collided = true;
        }

        double left_a = signed_distance_from_xz_boundary(cur.pos, left) - radius;
        double left_b = signed_distance_from_xz_boundary(next.pos, left) - radius;
        if (!collided && left_a > 0 && left_b <= 0)
        {
            double alpha = left_a / (left_a - left_b);
            render_pos = cur.pos + alpha * (next.pos - cur.pos);
            collided = true;
        }

        double floor_a = (cur.pos.y - radius) - floor.y;
        double floor_b = (next.pos.y - radius) - floor.y;
        if (!collided && floor_a > 0 && floor_b <= 0)
        {
            double alpha = floor_a / (floor_a - floor_b);
            render_pos = cur.pos + alpha * (next.pos - cur.pos);
            collided = true;
        }

        shape->set_transform(chain_transform({m, translation(render_pos.x, render_pos.y, render_pos.z)}));
        const Canvas img = render(camera, world);
        std::string ppm_string = canvas_to_ppm(img);
        std::ostringstream oss;
        oss << "frame_" << std::setw(4) << std::setfill('0') << i;
        std::string name = oss.str();
        ray_tracer::img::write_ppm(ppm_string, name);

        if (collided)
        {
            break;
        }
    }
}
} // namespace ray_tracer::animation