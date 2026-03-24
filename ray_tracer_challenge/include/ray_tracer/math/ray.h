#pragma once
#include <ray_tracer/math/tuple.h>

namespace ray_tracer::math {

    struct Matrix;
    
    struct Ray {
        Tuple origin;
        Tuple direction;
        Ray(const Tuple &origin, const Tuple &direction) : origin(origin), direction(direction) {}
        Tuple position(double t) const {
            return origin + (t * direction);
        }
    };

    Ray transform_ray(const Ray &ray, const Matrix &mat);

}