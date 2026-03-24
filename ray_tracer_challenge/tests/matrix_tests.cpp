#include <gtest/gtest.h>

#include "test_helpers.h"

using namespace ray_tracer::math;

TEST(Matrix, init) {
    const Matrix m(2, 2, {0.0, 1.0, 2.0, 3.0});
    expect_matrix(m, {0.0, 1.0, 2.0, 3.0});
}

TEST(Matrix, equality) {
    const Matrix a(2, 2, {0.0, 1.0, 2.0, 3.0});
    const Matrix b(2, 2, {0.0, 1.0, 2.0, 3.0});
    EXPECT_TRUE(a == b);
}

TEST(Matrix, multiplication) {
    const Matrix a(3, 3, {1, 2, 3, 3, 2, 1, 1, 2, 3});
    const Matrix b(3, 3, {4, 5, 6, 6, 5, 4, 4, 6, 5});
    expect_matrix(a * b, {28, 33, 29, 28, 31, 31, 28, 33, 29});
}

TEST(Matrix, identity) {
    const Matrix m(2, 2, {0.0, 1.0, 2.0, 3.0});
    const Matrix I = identity(2);
    expect_matrix(m * I, {0.0, 1.0, 2.0, 3.0});
}

TEST(Matrix, transpose) {
    const Matrix m(2, 2, {0.0, 1.0, 2.0, 3.0});
    const Matrix t = m.transpose();
    expect_matrix(t, {0.0, 2.0, 1.0, 3.0});
}

TEST(Matrix, transpose_rectangular) {
    const Matrix m(2, 3, {1, 2, 3, 4, 5, 6});
    const Matrix t = m.transpose();
    // expected shape: 3x2, data: {1,4,2,5,3,6}
    expect_matrix(t, {1, 4, 2, 5, 3, 6});
}

TEST(Matrix, determinant) {
    const Matrix m(2, 2, {5, 0, -1, 5});
    EXPECT_NEAR(determinant(m), 25, 1e-12);
    const Matrix m1(4, 4, {-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9});
    EXPECT_NEAR(determinant(m1), -4071, 1e-12);
    const Matrix m2(4, 4, {-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4});
    EXPECT_NEAR(determinant(m2), 532, 1e-12);
    // const Matrix m3(4, 4, {1, 0, 0, 0, 0, 0.707107, -0.707107, 0, 0, 0.707107, 0.707107, 0, 0, 0, 0, 1});
    // EXPECT_NEAR(determinant(m3), 1, 1e-12);
}

TEST(Matrix, submatrix) {
    const Matrix m(4, 4, {-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1});
    expect_matrix(m.submatrix(2, 1), {-6, 1, 6, -8, 8, 6, -7, -1, 1});
    const Matrix m1(4, 4, {1, 0, 0, 0, 0, 0.707107, -0.707107, 0, 0, 0.707107, 0.707107, 0, 0, 0, 0, 1});
}

TEST(Matrix, minor) {
    const Matrix m(3, 3, {3, 5, 0, 2, -1, -7, 6, -1, 5});
    double min = minor(m, 1, 0);
    EXPECT_NEAR(min, 25, 1e-12);
}

TEST(Matrix, cofactor) {
    const Matrix m(3, 3, {3, 5, 0, 2, -1, -7, 6, -1, 5});
    double cofactor1 = cofactor(m, 0, 0);
    double cofactor2 = cofactor(m, 1, 0);
    EXPECT_NEAR(cofactor1, -12, 1e-12);
    EXPECT_NEAR(cofactor2, -25, 1e-12);
}

TEST(Matrix, inverse) {
    std::vector<double> vals{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    const Matrix m(4, 4, {-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4});
    const Matrix m_inv = inverse_gauss_jordan(m);
    expect_matrix(m * m_inv, vals);
    const Matrix m2(4, 4, {8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4});
    const Matrix m2_inv = inverse_gauss_jordan(m2);
    expect_matrix(m2 * m2_inv, vals);
}

TEST(Matrix, matrix_max) {
    Matrix m(2, 2, {1, 2, 3, 4});
    dbl_eql(m.max(), 4);
    Matrix m2(3, 3, {1.0, 2.0, 3.0, 123.56, -10, 1, 2, 3, 4});
    dbl_eql(m2.max(), 123.56);
}

TEST(Matrix, check_pivot) {
    Matrix m(2, 2, {0, 1, 0, 1});
    EXPECT_EQ(check_pivot(m, 0, 1e-12), 0);
    Matrix m2(3, 3, {1, 0, 2, 3, 0, 4, 5, 0, 6});
    EXPECT_EQ(check_pivot(m2, 2, 1e-12), 1);
}

TEST(Matrix, normalize_row) {
    Matrix m(2, 2, {2, 1, 10, 100});
    normalize_row(m, 0);
    EXPECT_NEAR(m(0, 0), 1, 1e-9);
    EXPECT_NEAR(m(0, 1), 0, 5);
}

TEST(Matrix, inverse_gauss_jordan_not_invertible) {
    Matrix m(2, 2, {1, 1, 1, 1});
    EXPECT_THROW(inverse_gauss_jordan(m), std::runtime_error);
}

TEST(Matrix, inverse_gauss_jordan_identity) {
    Matrix m(3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1});
    const Matrix m_inv_ref(3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1});
    expect_matrix_eq(inverse_gauss_jordan(m), m_inv_ref);
}

TEST(Matrix, inverse_gauss_jordan_diagonal_mixed_4x4) {
    Matrix m(4, 4, {2, 0, 0, 0, 0, -3, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 10});
    const Matrix m_inv_ref(4, 4, {0.5, 0, 0, 0, 0, -0.3333333333333333, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0.1});
    expect_matrix_eq(inverse_gauss_jordan(m), m_inv_ref);
}

TEST(Matrix, inverse_gauss_jordan_permutation_3x3_self_inverse) {
    Matrix m(3, 3, {0, 0, 1, 0, 1, 0, 1, 0, 0});
    const Matrix m_inv_ref(3, 3, {0, 0, 1, 0, 1, 0, 1, 0, 0});
    expect_matrix_eq(inverse_gauss_jordan(m), m_inv_ref);
}

TEST(Matrix, inverse_gauss_jordan_negative_dominated_scale_2x2) {
    Matrix m(2, 2, {-1000, 0, 0, 1});
    const Matrix m_inv_ref(2, 2, {-0.001, 0, 0, 1});
    expect_matrix_eq(inverse_gauss_jordan(m), m_inv_ref);
}

TEST(Matrix, inverse_gauss_jordan_singular_duplicate_rows_throws) {
    Matrix m(3, 3, {1, 2, 3, 1, 2, 3, 4, 5, 6});
    EXPECT_THROW(inverse_gauss_jordan(m), std::runtime_error);
}

TEST(Matrix, inverse_gauss_jordan_nonsquare_throws) {
    Matrix m(2, 3, {1, 2, 3, 4, 5, 6});
    EXPECT_THROW(inverse_gauss_jordan(m), std::runtime_error);
}

TEST(Matrix, inverse_gauss_jordan) {
    Matrix m(3, 3, {2, -1, 0, -1, 2, -1, 0, -1, 2});
    const Matrix m_inv_ref(3, 3, {.75, .5, .25, .5, 1, .5, .25, .5, .75});
    expect_matrix_eq(inverse_gauss_jordan(m), m_inv_ref);
}