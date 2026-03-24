#pragma once
#include <ray_tracer/img/color.h>
#include <ray_tracer/math/tuple.h>
#include <ray_tracer/lighting/material.h>

namespace ray_tracer::geometry {
    class Shape;
}

namespace ray_tracer::lighting {

struct Light {
    math::Tuple position;
    img::Color intensity;
    Light() {}
    Light(const math::Tuple& position, const img::Color& intensity);
};

img::Color lighting(const Material& m,
                    const Light& light,
                    const math::Tuple& point,
                    const math::Tuple& eyev,
                    const math::Tuple& normalv,
                    const bool in_shadow
                    );

}  // namespace ray_tracer::lighting