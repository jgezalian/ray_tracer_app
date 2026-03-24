#pragma once
#include <ray_tracer/geometry/intersection.h>
#include <ray_tracer/geometry/shape.h>
#include <ray_tracer/helpers/computation.h>
#include <ray_tracer/lighting/light.h>

#include <vector>

namespace ray_tracer::helpers {
struct Computation;
}

namespace ray_tracer::world {

struct World {
        World() = default;
        World(const World& other) = delete;
        World(World&& other) noexcept;
        World& operator=(World&& other) noexcept;
        World& operator=(const World& other) = delete;
        ~World();

        void add(geometry::Shape *shape) { objects.push_back(shape); }
        std::vector<geometry::Shape *> objects;
        lighting::Light light;
        bool has_light = false;
        void cache_patterns();
};

World default_world();
ray_tracer::img::Color shade_hit(const World &world, const helpers::Computation &comps);
geometry::Intersection intersect_world_min(const World &world, const math::Ray &ray);
ray_tracer::img::Color color_at(const World &world,const math::Ray &ray);
bool is_shadowed(const math::Tuple &point, const world::World &world);
geometry::Intersection intersect_world_shadow(const World &world, const math::Ray &ray);


}  // namespace ray_tracer::world
