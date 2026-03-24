#include <ray_tracer/math/matrix.h>
#include <ray_tracer/math/tuple.h>

#include <cassert>
#include <iostream>
#include <utility>

using std::size_t;

namespace ray_tracer::math
{

Matrix::Matrix(size_t r, size_t c, std::initializer_list<double> values) : rows(r), cols(c)
{
    assert(r >= 0 && c >= 0 && "negative input size");
    if (values.size() != (r * c))
    {
        throw std::invalid_argument("inputted values size does not match matrix size");
    }
    data.assign(values.begin(), values.end());
}

Matrix::Matrix(size_t r, size_t c) : rows(r), cols(c), data((r * c), 0.0)
{
}

Matrix identity(size_t n)
{
    Matrix I(n, n);
    for (size_t i = 0; i < n; ++i)
    {
        I(i, i) = 1.0;
    }
    return I;
}

Matrix Matrix::transpose() const
{
    Matrix t(cols, rows);
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            t(j, i) = (*this)(i, j);
        }
    }
    return t;
};

bool operator==(const Matrix &a, const Matrix &b)
{
    if (a.cols != b.cols || a.rows != b.rows)
    {
        std::cout << "dimensions mismatch\n";
        return false;
    }
    if (a.data.size() != b.data.size())
    {
        std::cout << "data size mismatch\n";
        return false;
    }
    for (size_t i = 0; i < a.rows * a.cols; ++i)
    {
        if (!dbl_eql(a.data[i], b.data[i]))
        {
            std::cout << "data value mismatch at: " << "(" << ((i) / a.cols) << ", " << ((i % a.cols));
            return false;
        }
    }
    return true;
}

Matrix operator*(const Matrix &a, const Matrix &b)
{
    assert(a.cols == b.rows && "Matrices must be of form m x n, n x p");
    Matrix prod(a.rows, b.cols);
    for (size_t i = 0; i < a.rows; ++i)
    {
        for (size_t j = 0; j < b.cols; ++j)
        {
            double prod_sum = 0;
            for (size_t k = 0; k < a.cols; ++k)
            {
                prod_sum += (a(i, k) * b(k, j));
            }
            prod(i, j) = prod_sum;
        }
    }
    return prod;
}

Matrix Matrix::submatrix(size_t r, size_t c) const
{
    Matrix sub(rows - 1, cols - 1);
    size_t k = 0;
    size_t l = 0;
    for (size_t i = 0; i < rows; ++i)
    {
        if (i == r)
            continue;
        for (size_t j = 0; j < cols; ++j)
        {
            if (j == c)
                continue;
            sub(k, l) = (*this)(i, j);
            ++l;
        }
        l = 0;
        ++k;
    }
    return sub;
}

double minor(const Matrix &m, size_t r, size_t c)
{
    return determinant(m.submatrix(r, c));
}

double cofactor(const Matrix &m, size_t r, size_t c)
{
    double min = minor(m, r, c);
    if ((r + c) % 2 != 0)
        return -min;
    return min;
}

double determinant(const Matrix &m)
{
    assert(m.cols == m.rows && "Matrix is not square");
    if (m.cols == 2)
    {
        return (m(0, 0) * m(1, 1)) - (m(0, 1) * m(1, 0));
    };
    double det = 0;
    for (size_t i = 0; i < m.cols; ++i)
    {
        det += m(0, i) * cofactor(m, 0, i);
    }
    return det;
}

Matrix inverse(const Matrix &m)
{
    double det = determinant(m);
    assert(det != 0 && "Matrix is not invertible");
    Matrix m_inv(m.rows, m.cols);
    for (size_t i = 0; i < m.rows; ++i)
    {
        for (size_t j = 0; j < m.cols; ++j)
        {
            double cof = cofactor(m, i, j);
            m_inv(j, i) = cof / det;
        }
    }

    return m_inv;
}

double Matrix::max() const
{
    double max = 0.0;
    for (std::size_t r = 0; r < rows; ++r)
    {
        for (std::size_t c = 0; c < cols; ++c)
        {
            if ((*this)(r, c) > max)
                max = (*this)(r, c);
        }
    }
    return max;
}

double Matrix::max_abs() const
{
    double max = 0.0;
    for (std::size_t r = 0; r < rows; ++r)
    {
        for (std::size_t c = 0; c < cols; ++c)
        {
            if (std::abs((*this)(r, c)) > max)
                max = std::abs((*this)(r, c));
        }
    }
    return max;
}

Matrix inverse_gauss_jordan(const Matrix &m)
{
    if (m.rows != m.cols)
        throw std::runtime_error("Matrix must be square");
    Matrix I = identity(m.cols);
    size_t n = m.cols;
    Matrix augmented(m.rows, m.cols * 2);
    Matrix inverse(m.rows, m.cols);
    for (std::size_t i = 0; i < augmented.rows; ++i)
    {
        for (std::size_t j = 0; j < augmented.cols; ++j)
        {
            if (j > (m.cols - 1))
            {
                augmented(i, j) = I(i, j - m.cols);
            }
            else
                augmented(i, j) = m(i, j);
        }
    }

    double eps = 1e-15 + (1e-12 * m.max_abs()); // 1e-15 floor in case max == 0

    for (size_t k = 0; k < n; ++k)
    {
        if (!check_pivot(augmented, k, eps))
        { // check every pivot
            throw std::runtime_error("Matrix is not invertible");
        }
        normalize_row(augmented, k);
        eliminate(augmented, k);
    }

    for (std::size_t i = 0; i < inverse.rows; ++i)
    {
        for (std::size_t j = 0; j < inverse.cols; ++j)
        {
            inverse(i, j) = augmented(i, j + n);
        }
    }
    return inverse;
}

bool check_pivot(Matrix &m, size_t k, double eps)
{
    size_t row_with_max = k;
    size_t rows = m.rows;
    size_t cols = m.cols;
    double max = 0.0;
    for (size_t r = k; r < rows; ++r)
    { // r = pivot row
        double el = m(r, k);
        if (std::abs(el) > max)
        { // at first pivot value is max, then look for larger values to minimize rounding error
            max = std::abs(el);
            row_with_max = r;
        }
    }
    if (max < eps)
    { // no values found that meet threshold
        return false;
    }
    if (row_with_max == k)
    { // pivot value was max
        return true;
    }
    // swap row piv_i with row row_with_max
    for (size_t i = 0; i < cols; ++i)
    {
        std::swap(m(k, i), m(row_with_max, i));
    }
    return true;
}

void normalize_row(Matrix &m, std::size_t k)
{ // kth pivot
    std::size_t c = m.cols;
    double piv = m(k, k);
    for (std::size_t i = 0; i < c; ++i)
    { // iterate through row
        double cur = m(k, i);
        m(k, i) = cur / piv;
    }
}

void eliminate(Matrix &m, std::size_t k)
{
    for (std::size_t i = 0; i < m.rows; ++i)
    {
        if (i == k)
            continue;
        double factor = m(i, k);
        if (factor == 0.0)
            continue;
        for (std::size_t j = 0; j < m.cols; ++j)
        {
            m(i, j) += (-factor * m(k, j));
        }
    }
}

Tuple operator*(const Matrix &m, const Tuple &t)
{
    return (Tuple(((m(0, 0) * t.x) + (m(0, 1) * t.y) + (m(0, 2) * t.z) + (m(0, 3) * t.w)),
                  ((m(1, 0) * t.x) + (m(1, 1) * t.y) + (m(1, 2) * t.z) + (m(1, 3) * t.w)),
                  ((m(2, 0) * t.x) + (m(2, 1) * t.y) + (m(2, 2) * t.z) + (m(2, 3) * t.w)),
                  ((m(3, 0) * t.x) + (m(3, 1) * t.y) + (m(3, 2) * t.z) + (m(3, 3) * t.w))));

}

void print_matrix(const Matrix &m)
{
    for (size_t i = 0; i < m.rows; ++i)
    {
        for (size_t j = 0; j < m.cols; ++j)
        {
            std::cout << m(i, j) << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

} // namespace ray_tracer::math