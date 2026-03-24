#pragma once
#include <array>
#include <limits>
#include <ray_tracer/math/tuple.h>

namespace ray_tracer::physics
{
struct Params
{
    Params() : external_acceleration(math::vector(0, 0, 0))
    {
    }

    Params(math::Tuple extern_accel) : external_acceleration(extern_accel)
    {
    }
    math::Tuple external_acceleration;
};

struct Initial_Conditions
{
    Initial_Conditions() : init_velocity(math::vector(0, 0, 0)), init_coords(math::point(0, 0, 0))
    {
    }
    Initial_Conditions(const math::Tuple &init_velocity, const math::Tuple &init_coords);
    math::Tuple init_velocity;
    math::Tuple init_coords;
};
struct State : public Initial_Conditions
{
    State()
        : total_flight_time(std::numeric_limits<double>::max()), cur_coords(math::point(0, 0, 0)),
          cur_velocity(math::vector(0, 0, 0))
    {
    }
    double total_flight_time;
    math::Tuple cur_coords;
    math::Tuple cur_velocity;
    const math::Tuple state_at_time(double time_step, math::Tuple &init_velocity, math::Tuple &init_coords, Params &params);
};

// void set_total_flight_time(math::Tuple init_coords, math::Tuple velocity);

} // namespace ray_tracer::physics