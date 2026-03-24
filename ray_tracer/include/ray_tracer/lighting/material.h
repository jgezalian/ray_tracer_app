#pragma once
#include <ray_tracer/img/color.h>
#include <ray_tracer/lighting/pattern.h>



namespace ray_tracer::lighting {

struct Material{
    img::Color color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    double opacity;
    Pattern* pattern;
    Pattern* cached_pattern;
    math::Matrix pattern_and_object_transform;
    std::size_t pattern_transform_version;
    std::size_t object_transform_version;
        
    Material();
    //Material::Material(math::Matrix pattern_and_object_transform);
    Material(const Material& other_material);
    Material& operator=(const Material &other_material);
    ~Material() {delete pattern;}


};


}