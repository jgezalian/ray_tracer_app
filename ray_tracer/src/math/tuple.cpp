#include <ray_tracer/math/tuple.h>

namespace ray_tracer::math {

Tuple reflect(const Tuple &in, const Tuple &normal) {
    return (in - normal * 2 * dot(in, normal));
}

}