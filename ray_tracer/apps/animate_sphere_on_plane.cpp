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
#include <ray_tracer/physics/collision.h>
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
using namespace ray_tracer::collision;
using namespace ray_tracer::animation;

void animate(double v_x, double v_y, double v_z)
{
    World world{default_world()};
    for (auto *object : world.objects)
    {
        delete object;
    }
    world.objects.clear();
    Light light{point(0, 20, -5), Color{1, 1, 1}};
    world.light = light;

    Camera camera{1920, 1080, pi / 2};
    Tuple camera_origin{point(0, 5, -5)};
    camera.set_transform(camera_origin, point(0, 0, 10), vector(0, 1, 0));
    Floor coll_floor{0};
    Tuple start_pos = point(0, 1, 5);

    Ray bottom_left_ray = camera.ray_for_pixel(0, camera.vsize - 1);
    Ray upper_left_ray = camera.ray_for_pixel(0, 0);

    Ray bottom_right_ray = camera.ray_for_pixel(camera.hsize - 1, camera.vsize - 1);
    Ray upper_right_ray = camera.ray_for_pixel(camera.hsize - 1, 0);

    Tuple left_normal = cross(bottom_left_ray.direction, upper_left_ray.direction);
    Tuple right_normal = cross(bottom_right_ray.direction, upper_right_ray.direction);

    double intersect_t_bottom_left = (coll_floor.y - bottom_left_ray.origin.y) / bottom_left_ray.direction.y;
    double intersect_t_bottom_right = (coll_floor.y - bottom_right_ray.origin.y) / bottom_right_ray.direction.y;

    LineXZ left{bottom_left_ray.origin + intersect_t_bottom_left * bottom_left_ray.direction,
                norm(cross(left_normal, vector(0, 1, 0)))};

    LineXZ right{bottom_right_ray.origin + intersect_t_bottom_right * bottom_right_ray.direction,
                 norm(cross(right_normal, vector(0, 1, 0)))};

    Ray bottom_center_ray = camera.ray_for_pixel(camera.hsize / 2, camera.vsize - 1);
    double t = (coll_floor.y - bottom_center_ray.origin.y) / bottom_center_ray.direction.y;
    Tuple inside_floor_point = bottom_center_ray.origin + t * bottom_center_ray.direction;

    HalfSpaceXZ right_space{right, norm(cross(right.direction, vector(0, 1, 0)))};
    if (signed_distance_from_xz_boundary(inside_floor_point, right_space) < 0)
    {
        right_space.inward_normal = -right_space.inward_normal;
    }

    HalfSpaceXZ left_space{left, norm(cross(left.direction, vector(0, 1, 0)))};
    if (signed_distance_from_xz_boundary(inside_floor_point, left_space) < 0)
    {
        left_space.inward_normal = -left_space.inward_normal;
    }

    // floor
    Plane *floor = new Plane();
    floor->set_transform(translation(0, coll_floor.y, 0));
    Checkered_Pattern *cp = new Checkered_Pattern;
    cp->set_transform(scaling(4, 1, 4)); // bigger checkers on the plane
    floor->material.pattern = cp;
    world.objects.push_back(floor);

    // center sphere
    Sphere *center_sphere = new Sphere();
    Gradient_Pattern *gradient_pattern_center_sphere = new Gradient_Pattern({Color(0, 1, 0), Color(0, 0, 1.0)});
    gradient_pattern_center_sphere->set_transform(chain_transform({scaling(2, 2, 2), translation(-1, 0, 0)}));
    center_sphere->material.pattern = gradient_pattern_center_sphere;
    // double scale_x = 5;
    // double scale_y = 5;
    // double scale_z = 5;
    // Matrix scale{scaling(scale_x, scale_y, scale_z)};
    Matrix translate{translation(start_pos.x, start_pos.y, start_pos.z)};
    center_sphere->set_transform(chain_transform({translate}));
    world.objects.push_back(center_sphere);

    // Initial_Conditions init_cond{vector(v_x, v_y, v_z), center_sphere->get_transform() * point(0, 0, 0)};
    MotionState init{start_pos, vector(v_x, v_y, v_z)};
    Animation anim;
    MotionParams params{vector(0, -9.8, 0)};
    Matrix m{identity(4)};
    anim.animate_shape_raw_rgb(m, center_sphere, init, params, world, camera, right_space, left_space, coll_floor);
}

int main(int argc, char *argv[])
{
    std::string v_x_s = std::string((argv[1]));
    double v_x = std::stod(v_x_s);

    std::string v_y_s = std::string((argv[2]));
    double v_y = std::stod(v_y_s);

    std::string v_z_s = std::string((argv[3]));
    double v_z = std::stod(v_z_s);

    auto t1 = std::chrono::system_clock::now();
    animate(v_x, v_y, v_z);
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    return 0;
}
