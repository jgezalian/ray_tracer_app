#include </opt/homebrew/opt/libomp/include/omp.h>
#include <cstdint>
#include <fstream>
#include <ray_tracer/camera/camera.h>
#include <ray_tracer/geometry/intersection.h>
#include <ray_tracer/geometry/plane.h>
#include <ray_tracer/geometry/sphere.h>
#include <ray_tracer/img/canvas.h>
#include <ray_tracer/img/color.h>
#include <ray_tracer/lighting/light.h>
#include <ray_tracer/lighting/pattern.h>
#include <ray_tracer/math/matrix.h>
#include <ray_tracer/math/ray.h>
#include <ray_tracer/math/transform.h>
#include <ray_tracer/math/tuple.h>
#include <ray_tracer/world/world.h>

#include <chrono>
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

constexpr double pi = 3.14159265358979323846;

using namespace ray_tracer::world;
using namespace ray_tracer::camera;
using namespace ray_tracer::img;
using namespace ray_tracer::geometry;
using namespace ray_tracer::math;
using namespace ray_tracer::lighting;

void draw_scene()
{
    World world{default_world()};
    for (auto *object : world.objects)
    {
        delete object;
    }
    world.objects.clear();
    Light light{point(-5, 8, -10), Color{1, 1, 1}};
    world.light = light;

    // floor
    Plane *floor = new Plane();
    floor->set_transform(translation(0, -5, 0));
    Checkered_Pattern *cp = new Checkered_Pattern;
    cp->set_transform(scaling(4, 1, 4)); // bigger checkers on the plane
    floor->material.pattern = cp;
    world.objects.push_back(floor);

    // center sphere
    Sphere *center_sphere = new Sphere();
    Gradient_Pattern *gradient_pattern_center_sphere = new Gradient_Pattern({Color(0, 1, 0), Color(0, 0, 1.0)});
    gradient_pattern_center_sphere->set_transform(chain_transform({scaling(2, 2, 2), translation(-1, 0, 0)}));
    center_sphere->material.pattern = gradient_pattern_center_sphere;
    center_sphere->set_transform(chain_transform({translation(0, 1, -1), scaling(2, 2, 2)}));
    world.objects.push_back(center_sphere);

    Camera camera{1920, 1080, pi / 2};
    camera.set_transform(point(0, 2, -12), point(0, 0, 0), vector(0, 1, 0));
    const Canvas img = render(camera, world);
    std::string ppm_string = canvas_to_ppm(img);
    ray_tracer::img::write_ppm(ppm_string, "plane_sphere");
}

#ifdef __EMSCRIPTEN__
extern "C" void EMSCRIPTEN_KEEPALIVE sphere_on_plane_render_pixels(unsigned char *charPixels, std::size_t width,
                                                                   std::size_t height)
{
    World world{default_world()};
    for (auto *object : world.objects)
    {
        delete object;
    }
    world.objects.clear();
    Light light{point(-5, 8, -10), Color{1, 1, 1}};
    world.light = light;

    // floor
    Plane *floor = new Plane();
    floor->set_transform(translation(0, -5, 0));
    Checkered_Pattern *cp = new Checkered_Pattern;
    cp->set_transform(scaling(4, 1, 4)); // bigger checkers on the plane
    floor->material.pattern = cp;
    world.objects.push_back(floor);

    // wall
    // Plane* floor = new Plane();
    // floor->set_transform(chain_transform({rotate_x(pi / 2), translation(0, 0, 0)}));
    // Checkered_Pattern* cp = new Checkered_Pattern;
    // cp->set_transform(scaling(4, 1, 4));  // bigger checkers on the plane
    // floor->material.pattern = cp;
    // world.objects.push_back(floor);

    // center sphere
    Sphere *center_sphere = new Sphere();
    Gradient_Pattern *gradient_pattern_center_sphere = new Gradient_Pattern({Color(0, 1, 0), Color(0, 0, 1.0)});
    gradient_pattern_center_sphere->set_transform(chain_transform({scaling(2, 2, 2), translation(-1, 0, 0)}));
    center_sphere->material.pattern = gradient_pattern_center_sphere;
    center_sphere->set_transform(chain_transform({translation(0, 1, -1), scaling(3.3, 3.3, 3.3)}));
    world.objects.push_back(center_sphere);

    Camera camera{width, height, pi / 2};
    camera.set_transform(view_transform(point(0, 7, -12), point(0, 0, 0), vector(0, 1, 0)));
    const Canvas img = render(camera, world);
    int color_i = 0;
    for (auto &color : img.pixels)
    {
        charPixels[color_i] = std::fmin(255.0f, 255.0f * color.r);
        charPixels[color_i + 1] = std::fmin(255.0f, 255.0f * color.g);
        charPixels[color_i + 2] = std::fmin(255.0f, 255.0f * color.b);
        charPixels[color_i + 3] = 255;
        color_i += 4;
    }
}
#endif

int main()
{
    auto t1 = std::chrono::system_clock::now();
    draw_scene();
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    return 0;
}