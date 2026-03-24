#include <ray_tracer/math/ray.h>
#include <ray_tracer/math/matrix.h>

namespace ray_tracer::math {

    Ray transform_ray(const Ray &ray, const Matrix &mat) {
        return Ray{mat * ray.origin, mat * ray.direction};
    }


}
