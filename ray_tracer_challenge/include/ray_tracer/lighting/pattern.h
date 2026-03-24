#pragma once
#include <ray_tracer/img/color.h>
#include <ray_tracer/math/matrix.h>
#include <ray_tracer/math/transform.h>
#include <ray_tracer/math/tuple.h>

#include <vector>

namespace ray_tracer::geometry
{
class Shape;
}

namespace ray_tracer::lighting
{

class Pattern
{
  private:
    math::Matrix transform = math::identity(4);
    math::Matrix inverse_transform = math::inverse_gauss_jordan(transform);
    std::size_t transform_version;

  protected:
    Pattern()
        : transform(math::identity(4)), inverse_transform(math::inverse_gauss_jordan(transform)), transform_version(0),
          colors{img::Color(0, 0, 0), img::Color(1, 1, 1)}
    {
    }
    Pattern(std::initializer_list<img::Color> _colors)
        : transform(math::identity(4)), transform_version(0), colors(_colors)
    {
    }

  public:
    std::vector<img::Color> colors = {};
    virtual ~Pattern()
    {
    }
    virtual Pattern *clone() const = 0;
    void add_colors(std::initializer_list<img::Color> color_list);
    virtual img::Color pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform) = 0;
    math::Matrix get_transform()
    {
        return transform;
    };
    math::Matrix get_inverse_transform()
    {
        return inverse_transform;
    };
    void set_transform(const math::Matrix &mat)
    {
        transform = mat;
        inverse_transform = math::inverse_gauss_jordan(mat);
        ++transform_version;
    }
    std::size_t get_transform_version() const
    {
        return transform_version;
    }
};

struct Stripe_Pattern : public Pattern
{
    Stripe_Pattern() : Pattern()
    {
    }

    Stripe_Pattern(std::initializer_list<img::Color> _colors) : Pattern(_colors)
    {
    }

    img::Color pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform) override;

    Pattern *clone() const override
    {
        return new Stripe_Pattern(*this);
    }
};

struct Gradient_Pattern : public Pattern
{
    Gradient_Pattern() : Pattern()
    {
    }

    Gradient_Pattern(std::initializer_list<img::Color> _colors) : Pattern(_colors)
    {
    }

    img::Color pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform) override;

    Pattern *clone() const override
    {
        return new Gradient_Pattern(*this);
    }
};

struct Checkered_Pattern : public Pattern
{
    Checkered_Pattern() : Pattern()
    {
    }

    Checkered_Pattern(std::initializer_list<img::Color> _colors) : Pattern(_colors)
    {
    }

    img::Color pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform) override;

    Pattern *clone() const override
    {
        return new Checkered_Pattern(*this);
    }
};

struct Coordinate_Grid_Pattern : public Pattern
{
    Coordinate_Grid_Pattern() : Pattern()
    {
    }

    Coordinate_Grid_Pattern(std::initializer_list<img::Color> _colors) : Pattern(_colors)
    {
    }

    img::Color pattern_at_object(const math::Tuple &world_point, const math::Matrix &transform) override;

    Pattern *clone() const override
    {
        return new Coordinate_Grid_Pattern(*this);
    }
};

} // namespace ray_tracer::lighting
