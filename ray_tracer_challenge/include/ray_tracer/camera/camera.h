#pragma once
#include <ray_tracer/math/matrix.h>
#include <ray_tracer/math/transform.h>

#include <cmath>
#include <cstddef>

namespace ray_tracer::math {
struct Ray;
}

namespace ray_tracer::img {
struct Canvas;
}

namespace ray_tracer::world {
struct World;
}

namespace ray_tracer::camera {

struct Camera {
        std::size_t hsize = 0;
        std::size_t vsize = 0;
        double field_of_view = 0;
        double aspect = 0;
        double half_view = 0;
        double half_width = 0;
        double half_height = 0;
        double pixel_size = 0;
        double view_volume_x_left_bound = 0;
        double view_volume_x_right_bound = 0;
        double view_volume_y_lower_bound = 0;
        double view_volume_y_upper_bound = 0;
        double view_volume_z_near_bound = 0;

        Camera(std::size_t hsize_, std::size_t vsize_, double field_of_view_);

        math::Ray ray_for_pixel(const std::size_t x, const std::size_t y) const;

        void set_transform(const math::Tuple &from, const math::Tuple &to, const math::Tuple &up) {
            trans = math::view_transform(from, to, up);
            inverse_trans = math::inverse_gauss_jordan(trans);
            set_view_volume_bounds(from);
        }

        void set_view_volume_bounds(const math::Tuple &from) {
            view_volume_z_near_bound = from.z + 1;
            if (half_width >= half_height) {
                double view_volume_x_span = 2 * std::tan(field_of_view / 2) * std::abs(from.z);
                view_volume_x_left_bound = -0.5 * view_volume_x_span;
                view_volume_x_right_bound = 0.5 * view_volume_x_span;
                view_volume_y_lower_bound = -0.5 * (view_volume_x_span/aspect);
                view_volume_y_upper_bound = 0.5 * (view_volume_x_span/aspect);
            } 
            else {
                double view_volume_y_span = 2 * std::tan(field_of_view / 2) * std::abs(from.z);
                view_volume_x_left_bound = -0.5 * aspect * view_volume_y_span;
                view_volume_x_right_bound = 0.5 * aspect * view_volume_y_span;
                view_volume_y_lower_bound = -0.5 * view_volume_y_span;
                view_volume_y_upper_bound = 0.5 * view_volume_y_span;
            }
        }

        const math::Matrix &get_transform() const { return trans; }

        const math::Matrix &get_inverse_transform() const { return inverse_trans; }

    private:
        math::Matrix trans = math::identity(4);
        math::Matrix inverse_trans = math::inverse_gauss_jordan(trans);
};

img::Canvas render(const Camera &camera, world::World &world);
void binary_out(const Camera &camera, world::World &world, std::ofstream &out);

}  // namespace ray_tracer::camera