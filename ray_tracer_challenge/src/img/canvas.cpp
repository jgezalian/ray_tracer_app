#include <ray_tracer/img/canvas.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace ray_tracer::img {

// initalize canvas of width x height with all black pixels
Canvas::Canvas(std::size_t width, std::size_t height)
    : width(width),
      height(height),
      // pixels(height, std::vector<Color>(width, Color(0.0, 0.0, 0.0))) {}
      pixels(height * width, Color(0, 0, 0)) {}

// Color Canvas::pixel_at(std::size_t x, std::size_t y) const { return pixels[x][y]; }

Color Canvas::pixel_at(std::size_t x, std::size_t y) const { return pixels[y * width + x]; }

// void Canvas::write_pixel(std::size_t x, std::size_t y, const Color& c) {
//     if (y >= height || x >= width) return;
//     pixels[y][x] = c;
// }

void Canvas::write_pixel(std::size_t x, std::size_t y, const Color& c) {
    if (y >= height || x >= width) return;
    pixels[y * width + x] = c;
}

void write_ppm(const std::string& ppm_text, std::string name) {
    if (!std::filesystem::create_directory("generated_images")) {
        std::filesystem::create_directory("generated_images");
    }
    std::ofstream outfile("generated_images/" + name + ".ppm");
    outfile << ppm_text << '\n';
    outfile.close();
}

std::string canvas_to_ppm(const Canvas& canvas) {
    std::size_t row_len = 0;
    constexpr std::size_t MAX_CHAR_LINE = 70;
    std::size_t line_width = 0;
    std::string ppm_text{std::string("P3\n") + std::to_string(canvas.width) + std::string(" ") +
                         std::to_string(canvas.height) + std::string("\n255\n")};

    auto ppm_append = [&](int rgb_val) {
        if(row_len == canvas.width) {
            row_len = 0;
            ppm_text.push_back('\n');
            line_width = 0;
        }
        std::string next_token = std::to_string(limit255(rgb_val));
        std::size_t next_len =
            (line_width == 0 ? 0 : 1) + next_token.length(); /* +1 to count leading space when
                                                                writing past first val*/
        if (line_width + next_len > MAX_CHAR_LINE) {
            ppm_text.push_back('\n');
            line_width = 0;
        }
        if (line_width > 0) {
            ppm_text.push_back(' ');
            ++line_width;
        }
        ppm_text += next_token;
        line_width += next_token.length();
    };


    for (const auto& pixel : canvas.pixels) {
            int r = static_cast<uint8_t>(std::round(pixel.r * 255.0));
            int g = static_cast<uint8_t>(std::round(pixel.g * 255.0));
            int b = static_cast<uint8_t>(std::round(pixel.b * 255.0));
            ppm_append(r);
            ppm_append(g);
            ppm_append(b);
            ++row_len;
    }

    ppm_text.push_back('\n');

    return ppm_text;
}

}  // namespace ray_tracer::img
