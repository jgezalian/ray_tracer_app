#include <ray_tracer/math/tuple.h>
#include <ray_tracer/math/util.h>

#include <algorithm>

using ray_tracer::math::Tuple;

namespace ray_tracer::math {

bool dbl_eql(const double a, const double b) {
    double eps = 1e-12;
    return std::fabs(a - b) < eps;
}

double hypot(const Tuple &t) { return std::sqrt(std::pow((t.x), 2) + std::pow((t.y), 2) + std::pow(t.z, 2)); }

uint8_t color_to_uint8(double rgb_val) { return static_cast<uint8_t>(std::clamp(rgb_val, 0.0, 1.0) * 255); }
}  // namespace ray_tracer::math