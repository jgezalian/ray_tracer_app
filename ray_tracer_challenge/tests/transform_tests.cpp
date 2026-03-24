#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <numbers>

#include "test_helpers.h"

constexpr double pi = 3.14159265358979323846;
using namespace ray_tracer::math;

TEST(Transform, translate_point) {
    const Matrix trans = translation(5, -3, 2);
    const Tuple p = point(-3, 4, 5);
    const Matrix p_col_mat = Matrix(4, 1, {p.x, p.y, p.z, p.w});
    const Matrix p_trans_mat = trans * p_col_mat;
    const Tuple p_trans(p_trans_mat.data[0], p_trans_mat.data[1], p_trans_mat.data[2],
                        p_trans_mat.data[3]);
    expect_tuple(p_trans, 2, 1, 7, 1);
}

TEST(Transform, translate_point_inv) {
    const Matrix trans = translation(5, -3, 2);
    const Matrix trans_inv = inverse_gauss_jordan(trans);
    const Tuple p = point(-3, 4, 5);
    const Matrix p_col_mat = Matrix(4, 1, {p.x, p.y, p.z, p.w});
    const Matrix p_trans_mat = trans_inv * p_col_mat;
    const Tuple p_trans(p_trans_mat.data[0], p_trans_mat.data[1], p_trans_mat.data[2],
                        p_trans_mat.data[3]);
    expect_tuple(p_trans, -8, 7, 3, 1);
}

TEST(Transform, translate_vector) {
    const Matrix trans = translation(5, -3, 2);
    const Tuple v = vector(-3, 4, 5);
    const Matrix v_col_mat = Matrix(4, 1, {v.x, v.y, v.z, v.w});
    const Matrix v_trans_mat = trans * v_col_mat;
    const Tuple v_trans(v_trans_mat.data[0], v_trans_mat.data[1], v_trans_mat.data[2],
                        v_trans_mat.data[3]);
    expect_tuple(v_trans, -3, 4, 5, 0);
}

TEST(Transform, scale_point) {
    const Matrix scale = scaling(2, 3, 4);
    const Tuple p = point(-4, 6, 8);
    const Matrix p_col_mat = tuple_to_mat(p);
    const Matrix p_scale_mat = scale * p_col_mat;
    const Tuple p_scale(p_scale_mat.data[0], p_scale_mat.data[1], p_scale_mat.data[2],
                        p_scale_mat.data[3]);
    expect_tuple(p_scale, -8, 18, 32, 1);
}

TEST(Transform, scale_vec) {
    const Matrix scale = scaling(2, 3, 4);
    const Tuple v = vector(-4, 6, 8);
    const Matrix v_col_mat = tuple_to_mat(v);
    const Matrix v_trans_mat = scale * v_col_mat;
    const Tuple v_trans(v_trans_mat.data[0], v_trans_mat.data[1], v_trans_mat.data[2],
                        v_trans_mat.data[3]);
    expect_tuple(v_trans, -8, 18, 32, 0);
}

TEST(Transform, scale_vec_inv) {
    const Matrix scale = scaling(2, 3, 4);
    const Matrix scale_inv = inverse_gauss_jordan(scale);
    const Tuple v = vector(-4, 6, 8);
    const Matrix v_col_mat = tuple_to_mat(v);
    const Matrix v_trans_mat = scale_inv * v_col_mat;
    const Tuple v_trans(v_trans_mat.data[0], v_trans_mat.data[1], v_trans_mat.data[2],
                        v_trans_mat.data[3]);
    expect_tuple(v_trans, -2, 2, 2, 0);
}

TEST(Transform, reflection) {
    const Matrix scale = scaling(-1, 1, 1);
    const Tuple p = point(2, 3, 4);
    const Matrix p_col_mat = tuple_to_mat(p);
    const Matrix p_trans_mat = scale * p_col_mat;
    const Tuple p_trans(p_trans_mat.data[0], p_trans_mat.data[1], p_trans_mat.data[2],
                        p_trans_mat.data[3]);
    expect_tuple(p_trans, -2, 3, 4, 1);
}

TEST(Transform, rotate_x) {
    const Tuple p = point(0, 1, 0);
    const Matrix half_quarter = rotate_x(pi / 4);
    const Matrix full_quarter = rotate_x(pi / 2);
    expect_tuple(half_quarter * p, 0, std::sqrt(2) / 2, std::sqrt(2) / 2, 1);
    expect_tuple(full_quarter * p, 0, 0, 1, 1);
}

TEST(Transform, rotate_inv_x) {
    const Tuple p = point(0, 1, 0);
    const Matrix half_quarter = rotate_x(pi / 4);
    const Matrix inv_half_quarter = inverse_gauss_jordan(half_quarter);
    expect_tuple(inv_half_quarter * p, 0, std::sqrt(2) / 2, -1 * std::sqrt(2) / 2, 1);
}

TEST(Transform, rotate_y) {
    const Tuple p = point(0, 0, 1);
    const Matrix half_quarter = rotate_y(pi / 4);
    const Matrix full_quarter = rotate_y(pi / 2);
    expect_tuple(half_quarter * p, std::sqrt(2) / 2, 0, std::sqrt(2) / 2, 1);
    expect_tuple(full_quarter * p, 1, 0, 0, 1);
}

TEST(Transform, rotate_z) {
    const Tuple p = point(0, 1, 0);
    const Matrix half_quarter = rotate_z(pi / 4);
    const Matrix full_quarter = rotate_z(pi / 2);
    expect_tuple(half_quarter * p, -1 * std::sqrt(2) / 2, std::sqrt(2) / 2, 0, 1);
    expect_tuple(full_quarter * p, -1, 0, 0, 1);
}

TEST(Transform, shearing_xy) {
    const Matrix shear = shearing(1, 0, 0, 0, 0, 0);
    const Tuple p = point(2, 3, 4);
    expect_tuple(shear * p, 5, 3, 4, 1);
}

TEST(Transform, shearing_xz) {
    const Matrix shear = shearing(0, 1, 0, 0, 0, 0);
    const Tuple p = point(2, 3, 4);
    expect_tuple(shear * p, 6, 3, 4, 1);
}

TEST(Transform, shearing_yx) {
    const Matrix shear = shearing(0, 0, 1, 0, 0, 0);
    const Tuple p = point(2, 3, 4);
    expect_tuple(shear * p, 2, 5, 4, 1);
}

TEST(Transform, shearing_yz) {
    const Matrix shear = shearing(0, 0, 0, 1, 0, 0);
    const Tuple p = point(2, 3, 4);
    expect_tuple(shear * p, 2, 7, 4, 1);
}

TEST(Transform, shearing_zx) {
    const Matrix shear = shearing(0, 0, 0, 0, 1, 0);
    const Tuple p = point(2, 3, 4);
    expect_tuple(shear * p, 2, 3, 6, 1);
}

TEST(Transform, shearing_zy) {
    const Matrix shear = shearing(0, 0, 0, 0, 0, 1);
    const Tuple p = point(2, 3, 4);
    expect_tuple(shear * p, 2, 3, 7, 1);
}

TEST(Transform, sequence) {
    const Tuple p = point(1, 0, 1);
    const Matrix a = rotate_x(pi / 2);
    const Matrix b = scaling(5, 5, 5);
    const Matrix c = translation(10, 5, 7);

    const Tuple p2 = a * p;
    expect_tuple(p2, 1, -1, 0, 1);

    const Tuple p3 = b * p2;
    expect_tuple(p3, 5, -5, 0, 1);

    const Tuple p4 = c * p3;
    expect_tuple(p4, 15, 0, 7, 1);
}

TEST(Transform, chain) {
    const Tuple p = point(1, 0, 1);
    const Matrix a = rotate_x(pi / 2);
    const Matrix b = scaling(5, 5, 5);
    const Matrix c = translation(10, 5, 7);
    const Matrix chain = chain_transform({a, b, c});
    expect_tuple(chain * p, 15, 0, 7, 1);
}

TEST(Transform, view_transform_default) {
    Tuple from{point(0, 0, 0)};
    Tuple to{point(0, 0, -1)};
    Tuple up{vector(0, 1, 0)};
    Matrix t = view_transform(from, to, up);
    expect_matrix_eq(t, identity(4));
}

TEST(Transform, view_transform_pos_z) {
    Tuple from{point(0, 0, 0)};
    Tuple to{point(0, 0, 1)};
    Tuple up{vector(0, 1, 0)};
    Matrix t = view_transform(from, to, up);
    expect_matrix_eq(t, scaling(-1, 1, -1));
}

TEST(Transform, view_transform_world) {
    Tuple from{point(0, 0, 8)};
    Tuple to{point(0, 0, 0)};
    Tuple up{vector(0, 1, 0)};
    Matrix t = view_transform(from, to, up);
    expect_matrix_eq(t, translation(0, 0, -8));
}

TEST(Transform, view_transform_arbitrary) {
    Tuple from{point(1, 3, 2)};
    Tuple to{point(4, -2, 8)};
    Tuple up{vector(1, 1, 0)};
    Matrix t = view_transform(from, to, up);
    const Matrix expected(
        4, 4,
        {-0.50709, 0.50709, 0.67612, -2.36643, 0.76772, 0.60609, 0.12122, -2.82843, -0.35857,
         0.59761, -0.71714, 0.00000, 0.00000, 0.00000, 0.00000, 1.00000});
    expect_matrix_eq(t, expected);
}