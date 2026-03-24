#include <iostream>
#include <ray_tracer/animation/animation.h>
#include <ray_tracer/camera/camera.h>
#include <ray_tracer/geometry/shape.h>
#include <ray_tracer/img/canvas.h>
#include <ray_tracer/math/transform.h>
#include <ray_tracer/world/world.h>
#include <sstream>

using ray_tracer::camera::binary_out;
using ray_tracer::camera::Camera;
using ray_tracer::camera::render;
using ray_tracer::img::Canvas;
using ray_tracer::math::translation;
using ray_tracer::math::Tuple;
using ray_tracer::physics::Initial_Conditions;
using ray_tracer::physics::Params;
using ray_tracer::physics::State;
using ray_tracer::world::World;

namespace ray_tracer::animation
{

double constexpr eps = 1e-5;

void Animation::animate_shape_raw_rgb(geometry::Shape *shape, physics::State &state,
                                      physics::Initial_Conditions &init_cond, physics::Params &params, World &world,
                                      Camera &camera)
{
    Tuple v{math::vector(1, 1, 1)};
    v = shape->get_inverse_transform() * v;
    std::ofstream out("render.rgb", std::ios::binary);
    for (std::size_t i = 0; i < max_anim_time * fps; ++i)
    {
        Tuple cur_coords = state.state_at_time(i * time_step, init_cond.init_velocity, init_cond.init_coords, params);
        shape->set_transform(translation(cur_coords.x, cur_coords.y, cur_coords.z));

        binary_out(camera, world, out);

        double x_col = camera.view_volume_x_right_bound - (cur_coords.x + 1);
        double y_col = (cur_coords.y - 1) - camera.view_volume_y_lower_bound;
        if (i > 0 && ((x_col <= eps) || y_col <= eps)) // not at initial y = 0 and hit x/y boundary
        {
            break;
        }
    }
}

void Animation::animate_shape_ppm(geometry::Shape *shape, physics::State &state, physics::Initial_Conditions &init_cond,
                                  physics::Params &params, World &world, Camera &camera)
{
    Tuple v{math::vector(1, 1, 1)};
    v = shape->get_inverse_transform() * v;
    for (std::size_t i = 0; i < max_anim_time * fps; ++i)
    {
        Tuple cur_coords = state.state_at_time(i * time_step, init_cond.init_velocity, init_cond.init_coords, params);
        shape->set_transform(translation(cur_coords.x, cur_coords.y, cur_coords.z));
        const Canvas img = render(camera, world);
        std::string ppm_string = canvas_to_ppm(img);
        std::ostringstream oss;
        oss << "frame_" << std::setw(4) << std::setfill('0') << i;
        std::string name = oss.str();
        ray_tracer::img::write_ppm(ppm_string, name);

        double x_col = camera.view_volume_x_right_bound - (cur_coords.x + 1);
        double y_col = (cur_coords.y - 1) - camera.view_volume_y_lower_bound;
        if (i > 0 && ((x_col <= eps) || y_col <= eps)) // not at initial y = 0 and hit x/y boundary
        {
            break;
        }
    }
}
} // namespace ray_tracer::animation