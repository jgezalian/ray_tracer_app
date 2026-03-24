#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h> 
#include <ray_tracer/camera/camera.h>
#include <ray_tracer/geometry/intersection.h>
#include <ray_tracer/geometry/plane.h>
#include <ray_tracer/geometry/sphere.h>
#include <ray_tracer/helpers/computation.h>
#include <ray_tracer/img/canvas.h>
#include <ray_tracer/img/color.h>
#include <ray_tracer/lighting/light.h>
#include <ray_tracer/lighting/material.h>
#include <ray_tracer/math/matrix.h>
#include <ray_tracer/math/ray.h>
#include <ray_tracer/math/transform.h>
#include <ray_tracer/math/tuple.h>
#include <ray_tracer/world/world.h>

#include <vector>

// i should get refactor these global using qualifiers to function scope at some point
using ray_tracer::geometry::Intersection;
using ray_tracer::geometry::Sphere;
using ray_tracer::img::Color;
using ray_tracer::lighting::Light;
using ray_tracer::math::Matrix;
using ray_tracer::math::Tuple;

inline void expect_rgb(const Color& c, const double r, const double g, const double b,
                       const double eps = 1e-12) {
    EXPECT_NEAR(c.r, r, eps);
    EXPECT_NEAR(c.g, g, eps);
    EXPECT_NEAR(c.b, b, eps);
}

inline void expect_tuple(const Tuple& t, const double x, const double y, const double z,
                         const double w, const double eps = 1e-12) {
    EXPECT_NEAR(t.x, x, eps);
    EXPECT_NEAR(t.y, y, eps);
    EXPECT_NEAR(t.z, z, eps);
    EXPECT_NEAR(t.w, w, eps);
}

inline void tuple_eq(const Tuple& t1, const Tuple& t2, const double eps = 1e-5) {
    EXPECT_NEAR(t1.x, t2.x, eps);
    EXPECT_NEAR(t1.y, t2.y, eps);
    EXPECT_NEAR(t1.z, t2.z, eps);
    EXPECT_NEAR(t1.w, t2.w, eps);
}

inline void color_eq(const Color& c1, const Color& c2, const double eps = 1e-5) {
    EXPECT_NEAR(c1.r, c2.r, eps);
    EXPECT_NEAR(c1.g, c2.g, eps);
    EXPECT_NEAR(c1.b, c2.b, eps);
}

inline void expect_matrix(const Matrix& m, const std::vector<double>& values,
                          const double eps = 1e-12) {
    EXPECT_EQ(m.rows * m.cols, static_cast<int>(values.size()));
    for (std::size_t i = 0; i < m.rows * m.cols; ++i) {
        EXPECT_NEAR(m.data[i], values[i], eps);
    }
}

inline void expect_matrix_eq(const Matrix& a, const Matrix& b) {
    EXPECT_EQ(a.rows, b.rows);
    EXPECT_EQ(a.cols, b.cols);
    EXPECT_EQ(static_cast<int>(a.data.size()), static_cast<int>(b.data.size()));
    for (std::size_t i = 0; i < a.rows * a.cols; ++i) {
        EXPECT_NEAR(a.data[i], b.data[i], 1e-5);
    }
}

inline void expect_intersection(const Intersection i, double t, Sphere* sphere) {
    EXPECT_NEAR(i.t, t, 1e-12);
    EXPECT_EQ(i.shape->id, sphere->id);
}

inline void intersection_eq(const Intersection i1, const Intersection i2) {
    EXPECT_NEAR(i1.t, i2.t, 1e-12);
    EXPECT_EQ(i1.shape->id, i2.shape->id);
}

inline void light_eq(const Light l1, const Light l2) {
    tuple_eq(l1.position, l2.position);
    color_eq(l1.intensity, l2.intensity);
}
