#pragma once
#include <ray_tracer/math/tuple.h>
#include <ray_tracer/math/util.h>

#include <cassert>
#include <stdexcept>
#include <vector>

namespace ray_tracer::math {
struct Matrix {
        std::size_t rows;
        std::size_t cols;
        std::vector<double> data;

        Matrix(std::size_t r, std::size_t c, std::initializer_list<double> values);
        Matrix(std::size_t r, std::size_t c);

        const double &operator()(std::size_t r, std::size_t c) const { return data[r * cols + c]; }
        double &operator()(std::size_t r, std::size_t c) { return data[r * cols + c]; };
        Matrix transpose() const;
        Matrix submatrix(std::size_t r, std::size_t c) const;
        double max() const;
        double max_abs() const;
};

Matrix identity(std::size_t n);
bool operator==(const Matrix &a, const Matrix &b);
Matrix operator*(const Matrix &a, const Matrix &b);
Tuple operator*(const Matrix &m, const Tuple &t);
double minor(const Matrix &m, std::size_t r, std::size_t c);
double cofactor(const Matrix &m, std::size_t r, std::size_t c);
double determinant(const Matrix &m);
Matrix inverse(const Matrix &m);
Matrix inverse_gauss_jordan(const Matrix &m);
bool check_pivot(Matrix &m, std::size_t piv_i, double eps);
void print_matrix(const Matrix &m);
void normalize_row(Matrix &m, std::size_t k);
void eliminate(Matrix &m, std::size_t k);

};  // namespace ray_tracer::math
