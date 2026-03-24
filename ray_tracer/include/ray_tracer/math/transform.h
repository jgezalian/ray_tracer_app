#pragma once
#include <ray_tracer/math/matrix.h>

namespace ray_tracer::math {

Matrix translation(double x, double y, double z);
Matrix scaling(double x, double y, double z);
Matrix rotate_x(double r);
Matrix rotate_y(double r);
Matrix rotate_z(double r);
Matrix tuple_to_mat(const Tuple &t);
Matrix shearing(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y);
Matrix chain_transform(const std::vector<Matrix> &trans_matrices);
Matrix view_transform(const Tuple &from, const Tuple &to, const Tuple &up);

}