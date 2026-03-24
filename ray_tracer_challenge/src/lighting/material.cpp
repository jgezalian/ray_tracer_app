#include <ray_tracer/lighting/material.h>
using ray_tracer::img::Color;
using ray_tracer::math::identity;
namespace ray_tracer::lighting
{

Material::Material()
    : color(Color{1.0, 1.0, 1.0}), ambient(0.1), diffuse(0.9), specular(0.9), shininess(200), opacity(1),
      pattern(nullptr), cached_pattern(nullptr), pattern_and_object_transform(identity(4)),
      pattern_transform_version(0), object_transform_version(0)
{
}

Material::Material(const Material &other_material)
    : color(other_material.color), ambient(other_material.ambient), diffuse(other_material.diffuse),
      specular(other_material.specular), shininess(other_material.shininess), opacity(other_material.opacity),
      pattern(other_material.pattern ? other_material.pattern->clone() : nullptr),
      cached_pattern(nullptr),
      pattern_and_object_transform(other_material.pattern_and_object_transform),
      pattern_transform_version(other_material.pattern_transform_version),
      object_transform_version(other_material.object_transform_version)
{
}

Material &Material::operator=(const Material &other_material)
{
    if (this == &other_material)
        return *this;
    color = other_material.color;
    ambient = other_material.ambient;
    diffuse = other_material.diffuse;
    specular = other_material.specular;
    shininess = other_material.shininess;
    opacity = other_material.opacity;
    delete pattern;
    pattern = (other_material.pattern ? other_material.pattern->clone() : nullptr);
    cached_pattern = nullptr;
    pattern_and_object_transform = other_material.pattern_and_object_transform;
    pattern_transform_version = other_material.pattern_transform_version;
    object_transform_version = other_material.object_transform_version;

    return *this;
}

} // namespace ray_tracer::lighting
