#include <gtest/gtest.h>
#include "test_helpers.h"

using ray_tracer::img::Color;

TEST(Color, rgb_equal) {
    const Color a{1.0, 1.0, 1.0};
    expect_rgb(a, 1.0, 1.0, 1.0);
}

TEST(Color, add_colors) {
    const Color a{1.5, 5.5, 1.3};
    const Color b{3.5, 1.5, 3.3};
    const Color c = a + b;
    expect_rgb(c, 5.0, 7.0, 4.6);
}

TEST(Color, subtract_colors) {
    const Color a{1.5, 5.5, 1.3};
    const Color b{3.5, 1.5, 3.3};
    const Color c = a - b;
    expect_rgb(c, -2.0, 4.0, -2.0);
}

TEST(Color, multiply_scalar) {
    const Color a{1.0, 1.0, 1.0};
    constexpr int scalar = 5;
    const Color scaled = a * scalar;
    expect_rgb(scaled, 5.0, 5.0, 5.0);
}

TEST(Color, multiply_color) {
    const Color a{1.0, 2.0, 3.0};
    const Color b{3.0, 2.0, 1.0};
    const Color c = a * b;
    expect_rgb(c, 3.0, 4.0, 3.0);
}

TEST(Color, divide_by_scalar) {
    const Color a{4.0, 5.0, 6.0};
    const Color b = a / 2.0;
    expect_rgb(b, 2.0, 2.5, 3.0);
}

TEST(Color, negate_color) {
    const Color a{1.0, 1.0, 1.0};
    const Color b = -(a);
    expect_rgb(b, -1.0, -1.0, -1.0);
}

TEST(Color, color_eq) {
    const Color a{1.0, 1.5, 3.57};
    const Color b{1.0, 1.5, 3.57};
    EXPECT_NEAR(a.r, b.r, 1e-12);
    EXPECT_NEAR(a.g, b.g, 1e-12);
    EXPECT_NEAR(a.b, b.b, 1e-12);
}
