#include <iostream>
#include <ray_tracer/animation/animation.h>
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
#include <ray_tracer/physics/physics.h>
#include <ray_tracer/world/world.h>
#include <sstream>

constexpr double pi = 3.14159265358979323846;

using namespace ray_tracer::world;
using namespace ray_tracer::camera;
using namespace ray_tracer::img;
using namespace ray_tracer::geometry;
using namespace ray_tracer::math;
using namespace ray_tracer::lighting;
using namespace ray_tracer::physics;
using namespace ray_tracer::animation;

void animate()
{
    World world{default_world()};
    for (auto *object : world.objects)
    {
        delete object;
    }
    world.objects.clear();
    Light light{point(0, 5, -10), Color{1, 1, 1}};
    world.light = light;

    Camera camera{1920, 1080, pi / 6};
    const Tuple from = point(0, 0, -30);
    const Tuple to = point(0, 0, 0);
    const Tuple up = vector(0, 1, 0);
    camera.set_transform(from, to, up);

    // wall
    Plane *wall = new Plane();
    Coordinate_Grid_Pattern *grid = new Coordinate_Grid_Pattern;
    // grid->set_transform(translation(camera.view_volume_x_left_bound, 0, 0));
    wall->material.pattern = grid;
    wall->set_transform(chain_transform({rotate_x(pi / 2), translation(camera.view_volume_x_left_bound, 0, 0),
                                         translation(0, camera.view_volume_y_lower_bound, 0)}));
    world.objects.push_back(wall);

    Shape *center_sphere = new Sphere();
    center_sphere->material.color = Color(1.0, 0, 0);
    center_sphere->set_transform(chain_transform(
        {scaling(1, 1, 1), translation(camera.view_volume_x_left_bound + 1, camera.view_volume_y_lower_bound + 1, 0)}));
    world.add(center_sphere);

    const Canvas img = render(camera, world);
    std::string ppm_string = canvas_to_ppm(img);
    ray_tracer::img::write_ppm(ppm_string, "scratch");
}

int main()
{
    auto t1 = std::chrono::system_clock::now();
    animate();
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    return 0;
}
