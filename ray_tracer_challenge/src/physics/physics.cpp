#include <ray_tracer/physics/physics.h>

namespace ray_tracer::physics
{

Initial_Conditions::Initial_Conditions(const math::Tuple &_init_velocity, const math::Tuple &_init_coords)
    : init_velocity(_init_velocity), init_coords(_init_coords)
{
}

// void set_total_flight_time(math::Tuple init_coords, math::Tuple velocity)
// {
// }

const math::Tuple State::state_at_time(double time_step, math::Tuple &init_velocity, math::Tuple &init_coords, Params &params)
{

    return cur_coords = (time_step * init_velocity) + (0.5 * params.external_acceleration * (time_step * time_step))  + init_coords;
}
} // namespace ray_tracer::physics
