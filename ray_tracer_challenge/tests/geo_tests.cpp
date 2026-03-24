#include "test_helpers.h"

using namespace ray_tracer::math;

TEST(Geometry, 3d_point_eq) {
    const Tuple t = point(1.0, 1.0, 1.0);
    expect_tuple(t, 1.0, 1.0, 1.0, 1.0);
}

TEST(Geometry, 3d_vec_eq) {
    const Tuple t = vector(1.0, 1.0, 1.0);
    expect_tuple(t, 1.0, 1.0, 1.0, 0.0);
}

TEST(Geometry, is_point) {
    const Tuple t = point(0.0, 0.0, 0.0);
    EXPECT_EQ(t.w, 1.0);
}

TEST(Geometry, is_vector) {
    const Tuple t = vector(0.0, 0.0, 0.0);
    EXPECT_EQ(t.w, 0.0);
}

TEST(Geometry, tuple_add) {
    const Tuple p{0.0, 0.0, 0.0, 1.0};
    const Tuple v{1.0, 2.0, 3.0, 0.0};
    const Tuple add = p + v;
    expect_tuple(add, 1.0, 2.0, 3.0, 1.0);
}

TEST(Geometry, tuple_sub) {
    const Tuple p{0.0, 0.0, 0.0, 1.0};
    const Tuple v{1.0, 2.0, 3.0, 0.0};
    const Tuple sub = p - v;
    expect_tuple(sub, -1.0, -2.0, -3.0, 1.0);
}

TEST(Geometry, tuple_negate) {
    const Tuple t{-1.0, 1.0, 0.1, 0.0};
    expect_tuple(-(t), 1.0, -1.0, -0.1, 0.0);
}

TEST(Geometry, tuple_mult) {
    const Tuple t1{-1.0, 2.0, -3.0, 1.5};
    const double scalar = -0.5;
    const Tuple mult = t1 * scalar;
    expect_tuple(mult, 0.5, -1.0, 1.5, -0.75);
}

TEST(Geometry, tuple_div) {
    const Tuple t1{-1.0, 2.0, -3.0, 1.5};
    const double scalar = -0.5;
    const Tuple div = t1 / scalar;
    expect_tuple(div, 2.0, -4.0, 6.0, -3.0);
}

TEST(Geometry, tuple_mag) {
    const Tuple t{1.0, 5.5, 3.7, 0.0};
    EXPECT_NEAR(magnitude(t), std::hypot(t.x, t.y, t.z), 1e-12);
}

TEST(Geometry, tuple_norm) {
    const Tuple t{3.0, 3.0, 3.0, 0.0};
    expect_tuple(norm(t), 0.57735026918962573, 0.57735026918962573, 0.57735026918962573, 0.0);
}

TEST(Geometry, tuple_dot) {
    const Tuple v1{1.0, 2.0, 3.0, 0.0};
    const Tuple v2{-1.0, -2.0, -3.0, 0.0};
    EXPECT_NEAR(dot(v1, v2), -14.0, 1e-12);
}

TEST(Geometry, tuple_cross) {
    const Tuple v1{1.0, 2.0, 3.0, 0.0};
    const Tuple v2{-1.0, -2.0, -3.0, 0.0};
    expect_tuple(cross(v1, v2), 0.0, 0.0, 0.0, 0.0);
}

TEST(Geometry, reflect) {
    const Tuple v = vector(1, -1, 0);
    const Tuple n = vector(0, 1, 0);
    Tuple r = reflect(v, n);
    tuple_eq(r, vector(1, 1, 0));
}

TEST(Geometry, reflect_slanted) {
    const Tuple v = vector(0, -1, 0);
    const Tuple n = vector(std::sqrt(2)/2, std::sqrt(2)/2, 0);
    Tuple r = reflect(v, n);
    tuple_eq(r, vector(1, 0, 0));
}
