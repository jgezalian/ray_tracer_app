#include <gtest/gtest.h>

#include "test_helpers.h"

using namespace ray_tracer::lighting;
using namespace ray_tracer::math;
using namespace ray_tracer::geometry;

Sphere sphere;

TEST(Lighting, constructor) {
    const Color intensity(1.0, 1.0, 1.0);
    const Tuple position = point(0, 0, 0);
    const Light l{position, intensity};
    tuple_eq(l.position, position);
    color_eq(l.intensity, intensity);
}

TEST(Lighting, material_constructor) {
    const Material mat{};
    color_eq(mat.color, Color{1.0, 1.0, 1.0});
    EXPECT_NEAR(mat.ambient, 0.1, 1e-12);
    EXPECT_NEAR(mat.diffuse, 0.9, 1e-12);
    EXPECT_NEAR(mat.specular, 0.9, 1e-12);
    EXPECT_NEAR(mat.shininess, 200, 1e-12);
}

// TEST(Lighting, eye_between_light_and_surface) {
//     const Tuple eyev = vector(0, 0, -1);
//     const Tuple normalv = vector(0, 0, -1);
//     const Material mat{};
//     const Light light{point(0, 0, -10), Color{1, 1, 1}};
//     const Tuple p{point(0, 0, 0)};
//     Color res = lighting(&sphere, mat, light, p, eyev, normalv, false);
//     color_eq(res, Color{1.9, 1.9, 1.9});
// }

// TEST(Lighting, eye_between_light_and_surface_offset45) {
//     const Tuple eyev = vector(0, std::sqrt(2) / 2, -1 * std::sqrt(2) / 2);
//     const Tuple normalv = vector(0, 0, -1);
//     const Material mat{};
//     const Light light{point(0, 0, -10), Color{1, 1, 1}};
//     const Tuple p{point(0, 0, 0)};
//     Color res = lighting(&sphere, mat, light, p, eyev, normalv, false);
//     color_eq(res, Color{1, 1, 1});
// }

// TEST(Lighting, eye_opposite_surface_offset45) {
//     const Tuple eyev = vector(0, 0, -1);
//     const Tuple normalv = vector(0, 0, -1);
//     const Material mat{};
//     const Light light{point(0, 10, -10), Color{1, 1, 1}};
//     const Tuple p{point(0, 0, 0)};
//     Color res = lighting(&sphere, mat, light, p, eyev, normalv, false);
//     color_eq(res, Color{0.7364, 0.7364, 0.7364});
// }

// TEST(Lighting, eye_in_path_reflection_vector) {
//     const Tuple eyev = vector(0, -1 * std::sqrt(2) / 2, -1 * std::sqrt(2) / 2);
//     const Tuple normalv = vector(0, 0, -1);
//     const Material mat{};
//     const Light light{point(0, 10, -10), Color{1, 1, 1}};
//     const Tuple p{point(0, 0, 0)};
//     Color res = lighting(&sphere, mat, light, p, eyev, normalv, false);
//     color_eq(res, Color{1.6364, 1.6364, 1.6364});
// }

// TEST(Lighting, light_behind_surface) {
//     const Tuple eyev = vector(0, 0, -1);
//     const Tuple normalv = vector(0, 0, -1);
//     const Material mat{};
//     const Light light{point(0, 0, 10), Color{1, 1, 1}};
//     const Tuple p{point(0, 0, 0)};
//     Color res = lighting(&sphere, mat, light, p, eyev, normalv, true);
//     color_eq(res, Color{0.1, 0.1, 0.1});
// }

// TEST(Lighting, lighting_with_surface_in_shadow) {
//     const Tuple eyev = vector(0, 0, -1);
//     const Tuple normalv = vector(0, 0, -1);
//     const Material mat{};
//     const Light light{point(0, 0, -10), Color{1, 1, 1}};
//     const Tuple p{point(0, 0, 0)};
//     Color res = lighting(&sphere, mat, light, p, eyev, normalv, true);
//     color_eq(res, Color{0.1, 0.1, 0.1});
// }

// TEST(Lighting, stripes_with_object_trans) {
//     Sphere sphere;
//     sphere.material.pattern = new Stripe_Pattern;
//     sphere.set_transform(scaling(2, 2, 2));
//     //point (1,1,1) -> (.5, .5, .5) after transformation, floor(0.5) == 0, 0 % 2 == 0, color[0] == black
//     color_eq(sphere.material.pattern->pattern_at_object(point(1, 1, 1), &sphere), Color(0, 0, 0));
// }

// TEST(Lighting, stripes_with_pattern_trans) {
//     Sphere sphere;
//     Stripe_Pattern* stripe_pattern = new Stripe_Pattern;
//     stripe_pattern->set_transform(scaling(2, 2, 2));
//     sphere.material.pattern = stripe_pattern;
//     color_eq(sphere.material.pattern->pattern_at_object(point(1, 1, 1), &sphere), Color(0, 0, 0));
// }

// TEST(Lighting, stripes_with_pattern_and_obj_trans) {
//     Sphere sphere;
//     Stripe_Pattern* stripe_pattern = new Stripe_Pattern;
//     stripe_pattern->set_transform(scaling(0.5, 0.5, 0.5));
//     sphere.material.pattern = stripe_pattern;
//     sphere.set_transform(translation(-0.5, 0, 0));
//     color_eq(sphere.material.pattern->pattern_at_object(point(1, 1, 1), &sphere), Color(1, 1, 1));
// }