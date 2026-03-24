#include <ray_tracer/math/matrix.h>

using namespace ray_tracer::math;

int main() {
    Matrix m(3, 3, {0, 2, 3, 4, 0, 6, 7, 8, 0});
    inverse_gauss_jordan(m);
    return 0;
}

