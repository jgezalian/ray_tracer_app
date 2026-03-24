#include <gtest/gtest.h>

#include <iostream>

#include "test_helpers.h"

using ray_tracer::img::Canvas;
using ray_tracer::img::Color;
using namespace std::string_literals;

TEST(Canvas, init_black) {
    Canvas canvas{100, 100};
    std::size_t h = canvas.height;
    std::size_t w = canvas.width;
    EXPECT_EQ(h, 100);
    EXPECT_EQ(w, 100);

    for (std::size_t i = 0; i < h; ++i) {
        for (std::size_t j = 0; j < w; ++j) {
            expect_rgb(canvas.pixel_at(i, j), 0.0, 0.0, 0.0);
        }
    }
}

TEST(Canvas, write_pixel) {
    Canvas canvas{100, 100};
    Color color{1.0, 1.0, 1.0};
    canvas.write_pixel(99, 99, color);
    expect_rgb(canvas.pixel_at(99, 99), 1.0, 1.0, 1.0);
}

TEST(Canvas, canvas_to_ppm) {
    Canvas canvas{5, 3};
    Color c1{1.5, 0.0, 0.0};
    Color c2{0.0, 0.5, 0.0};
    Color c3{-0.5, 0.0, 1};
    canvas.write_pixel(0, 0, c1);
    canvas.write_pixel(2, 1, c2);
    canvas.write_pixel(4, 2, c3);
    std::string ppm_text = canvas_to_ppm(canvas);
    std::string expected_string{
        "P3\n"s
        "5 3\n"
        "255\n"
        "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n"};

    EXPECT_EQ(ppm_text, expected_string);
}

TEST(Canvas, end_new_line) {
    Canvas canvas{1, 1};
    std::string ppm_text = canvas_to_ppm(canvas);
    EXPECT_EQ(ppm_text.back(), '\n');
}
