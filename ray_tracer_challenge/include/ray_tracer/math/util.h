#pragma once
#include <cmath>
namespace ray_tracer::math {
struct Tuple;
bool dbl_eql(const double a, const double b);
double hypot(const ray_tracer::math::Tuple &t);
uint8_t color_to_uint8(double rgb_val);
}  // namespace ray_tracer::math