#include <ray_tracer/geometry/sphere.h>
#include <ray_tracer/math/transform.h>
#include <ray_tracer/world/world.h>
#include <utility>

namespace ray_tracer::world
{

// using geometry::intersect;
using geometry::Intersection;
using geometry::Sphere;
using helpers::Computation;
using helpers::prepare_computation;
using img::Color;
using lighting::Light;
using lighting::lighting;
using math::magnitude;
using math::norm;
using math::point;
using math::Ray;
using math::scaling;
using math::Tuple;

World::~World()
{
    for (auto *object : objects)
    {
        delete object;
    }
}

World::World(World &&other) noexcept
    : objects(std::move(other.objects)), light(std::move(other.light)), has_light(other.has_light)
{
    other.objects.clear();
    other.has_light = false;
}
World &World::operator=(World &&other) noexcept
{
    if (this == &other)
        return *this;
    for (auto *obj : objects)
    {
        delete obj;
    }
    objects.clear();
    objects = std::move(other.objects);
    light = std::move(other.light);
    has_light = other.has_light;

    other.objects.clear();
    other.has_light = false;
    return *this;
}

World default_world()
{
    World world{};
    Light light{point(-10, 10, -10), Color{1, 1, 1}};
    world.has_light = true;
    Sphere *s1 = new Sphere();
    s1->material.color = Color{0.8, 1.0, 0.6};
    s1->material.diffuse = 0.7;
    s1->material.specular = 0.2;
    Sphere *s2 = new Sphere();
    s2->set_transform(scaling(0.5, 0.5, 0.5));

    world.add(s1);
    world.add(s2);
    world.light = light;

    return world;
}

Intersection intersect_world_min(const World &world, const Ray &ray)
{
    Intersection min_intersection = Intersection(__DBL_MAX__, nullptr);
    if (world.objects.size() == 0)
        return min_intersection;
    for (const auto *object : world.objects)
    {
        Intersection i = object->get_min_intersection(ray);
        if (i.t < min_intersection.t && i.t > 0)
            min_intersection = i;
    }
    return min_intersection;
}

Intersection intersect_world_shadow(const World &world, const Ray &ray)
{
    Intersection min_intersection = Intersection(__DBL_MAX__, nullptr);
    for (const auto *object : world.objects)
    {
        Intersection i = object->get_min_intersection(ray);
        if (i.t < min_intersection.t && i.t > 0)
        {
            min_intersection = i;
        }
    }
    return min_intersection;
}

Color shade_hit(const World &world, const helpers::Computation &comps)
{
    bool in_shadow = is_shadowed(comps.over_point, world);
    return lighting(comps.shape->material, world.light, comps.point, comps.eyev, comps.normalv, in_shadow);
}

Color color_at(const World &world, const math::Ray &ray)
{
    Intersection min_intersection = intersect_world_min(world, ray);
    if (min_intersection.t == __DBL_MAX__)
        return Color(0, 0, 0);
    Computation comps = prepare_computation(min_intersection, ray);
    return shade_hit(world, comps);
}

bool is_shadowed(const math::Tuple &point, const world::World &world)
{
    const double dist = magnitude(world.light.position - point);
    Ray ray{point, norm(world.light.position - point)};

    Intersection min_intersection = Intersection(__DBL_MAX__, nullptr);
    for (const auto *object : world.objects)
    {
        Intersection i = object->get_min_intersection(ray);
        if (i.t < min_intersection.t && i.t > 0 && i.t < dist)
        {
            return true;
        }
    }
    return false;
}

void World::cache_patterns()
{
    for (auto *object : objects)
    {
        if (object->material.pattern)
        {
            if (object->get_transform_version() != object->material.object_transform_version ||
                object->material.pattern->get_transform_version() != object->material.pattern_transform_version ||
                object->material.pattern != object->material.cached_pattern)
            {
                object->material.pattern_and_object_transform =
                    object->material.pattern->get_inverse_transform() * object->get_inverse_transform();
                object->material.cached_pattern = object->material.pattern;

                object->material.pattern_transform_version = object->material.pattern->get_transform_version();
                object->material.object_transform_version = object->get_transform_version();
            }
        }
    }
}

} // namespace ray_tracer::world