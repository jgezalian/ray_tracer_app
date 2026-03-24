#include <ray_tracer/math/transform.h>

namespace ray_tracer::math {
    
Matrix translation(double x, double y, double z) {
    Matrix trans(identity(4));
    trans(0, 3) = x;
    trans(1, 3) = y;
    trans(2, 3) = z;

    return trans;
}

Matrix scaling(double x, double y, double z) {
    Matrix scale(identity(4));
    scale(0, 0) = x;
    scale(1, 1) = y;
    scale(2, 2) = z;

    return scale;
}

Matrix rotate_x(double r) {
    Matrix rot_x(identity(4));
    rot_x(1, 1) = std::cos(r);
    rot_x(1, 2) = -1 * std::sin(r);
    rot_x(2, 1) = std::sin(r);
    rot_x(2, 2) = std::cos(r);

    return rot_x;
}

Matrix rotate_y(double r) {
    Matrix rot_y(identity(4));
    rot_y(0, 0) = std::cos(r);
    rot_y(0, 2) = std::sin(r);
    rot_y(2, 0) = -1 * std::sin(r);
    rot_y(2, 2) = std::cos(r);

    return rot_y;
}

Matrix rotate_z(double r) {
    Matrix rot_z(identity(4));
    rot_z(0, 0) = std::cos(r);
    rot_z(0, 1) = -1 * std::sin(r);
    rot_z(1, 0) = std::sin(r);
    rot_z(1, 1) = std::cos(r);

    return rot_z;
}

Matrix shearing(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y) {
    Matrix shear(identity(4));
    shear(0, 1) = x_y;
    shear(0, 2) = x_z;
    shear(1, 0) = y_x;
    shear(1, 2) = y_z;
    shear(2, 0) = z_x;
    shear(2, 1) = z_y;

    return shear;
}

Matrix chain_transform(const std::vector<Matrix> &trans_matrices) {
    Matrix matrix_chain(identity(4));
    for(auto it = trans_matrices.rbegin(); it != trans_matrices.rend(); ++it) {
        matrix_chain = matrix_chain * *it;
    }
    return matrix_chain;
};

Matrix tuple_to_mat(const Tuple &t) {
    const Matrix t_col_mat = Matrix(4, 1, {t.x, t.y, t.z, t.w});
    return t_col_mat;
}

Matrix view_transform(const Tuple &from, const Tuple &to, const Tuple &up) {
    Matrix orientation(identity(4));
    const Tuple forward = norm(to - from);
    const Tuple left = cross(forward, norm(up));
    const Tuple true_up = cross(left, forward);
    orientation(0, 0) = left.x;
    orientation(0, 1) = left.y;
    orientation(0, 2) = left.z;
    orientation(1, 0) = true_up.x;
    orientation(1, 1) = true_up.y;
    orientation(1, 2) = true_up.z;
    orientation(2, 0) = -1 * forward.x;
    orientation(2, 1) = -1 * forward.y;
    orientation(2, 2) = -1 * forward.z;
    return(orientation * translation(-1 * from.x, -1 * from.y, -1 * from.z));
}


}  // namespace ray_tracer::math