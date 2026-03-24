#include <ray_tracer/geometry/shape.h>
#include <ray_tracer/lighting/pattern.h>

using ray_tracer::img::Color;
using ray_tracer::math::chain_transform;
using ray_tracer::math::Matrix;
using ray_tracer::math::print_tuple;
using ray_tracer::math::Tuple;

using ray_tracer::geometry::Shape;
using ray_tracer::math::Tuple;

namespace ray_tracer::lighting
{

void Pattern::add_colors(std::initializer_list<Color> color_list)
{
    for (auto color : color_list)
    {
        colors.push_back(color);
    }
}

Color Stripe_Pattern::pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform)
{
    Tuple pattern_point = transform * world_point;
    int int_pattern_point = static_cast<int>(std::abs(std::floor(pattern_point.x)));
    return colors[static_cast<std::size_t>(int_pattern_point) % colors.size()];
}

Color Gradient_Pattern::pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform)
{
    Tuple pattern_point = transform * world_point;
    double c = (pattern_point.x - std::floor(pattern_point.x));
    return colors[0] + ((colors[1] - colors[0]) * c);
}

Color Checkered_Pattern::pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform)
{
    Tuple pattern_point = transform * world_point;
    int sum = static_cast<int>((std::floor(pattern_point.x)) + (std::floor(pattern_point.z)));
    if (sum % 2 == 0)
    {
        return colors[0];
    }
    return colors[1];
}

Color Coordinate_Grid_Pattern::pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform)
{
    Tuple pattern_point = transform * world_point;
    if (pattern_point.x >= -0.1 && pattern_point.x <= 0.1 && pattern_point.z >= -0.1 && pattern_point.z <= 0.1)
    {
        return Color(1.0, 0, 0);
    }
    return Color(1, 1, 1);
}

} // namespace ray_tracer::lighting