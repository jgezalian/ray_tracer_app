#pragma once
#include <ray_tracer/math/tuple.h>

namespace ray_tracer::demo {

struct Projectile {
    ray_tracer::math::Tuple pos;
    ray_tracer::math::Tuple vel;

    Projectile(const ray_tracer::math::Tuple& pos, const ray_tracer::math::Tuple& vel)
        : pos(pos), vel(vel) {}
};

struct Environment {
    ray_tracer::math::Tuple grav;
    ray_tracer::math::Tuple wind;

    Environment(const ray_tracer::math::Tuple& grav, const ray_tracer::math::Tuple& wind)
        : grav(grav), wind(wind) {}
};

inline Projectile launch(const Projectile& p, const Environment& e) {
    ray_tracer::math::Tuple pos_i = p.pos + p.vel;
    ray_tracer::math::Tuple vel_i = p.vel + e.grav + e.wind;
    return Projectile(pos_i, vel_i);
}
}  // namespace ray_tracer::demo