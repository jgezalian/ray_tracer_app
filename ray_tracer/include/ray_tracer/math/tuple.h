#pragma once
#include <ray_tracer/math/util.h>
#include <cassert>
#include <cmath>
#include <iostream>

namespace ray_tracer::math {
struct Tuple {
    double x;
    double y;
    double z;
    double w;
    static constexpr double epsilon = 1e-5;

    Tuple() : x(0), y(0), z(0), w(0) {}
    Tuple(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    // if w == 1, is point
    constexpr bool isPoint() const noexcept { return std::abs(w - 1.0) < epsilon; }

    // if w == 0, is vector
    constexpr bool isVector() const noexcept { return std::abs(w - 0.0) < epsilon; }
};


inline void print_tuple(const Tuple& t) {
    std::cout << " x: " << t.x << " y: " << t.y << " z: " << t.z << " w: " << t.w << '\n';
}

inline Tuple point(double x, double y, double z) { return Tuple(x, y, z, 1.0); }

inline Tuple vector(double x, double y, double z) { return Tuple(x, y, z, 0.0); }

inline Tuple operator+(const Tuple& t1, const Tuple& t2) {
    return Tuple(t1.x + t2.x, t1.y + t2.y, t1.z + t2.z, t1.w + t2.w);
}

inline Tuple operator-(const Tuple& t1, const Tuple& t2) {
    return Tuple(t1.x - t2.x, t1.y - t2.y, t1.z - t2.z, t1.w - t2.w);
}

inline Tuple operator-(const Tuple& t) { return Tuple(-t.x, -t.y, -t.z, -t.w); }

inline Tuple operator*(const Tuple& t, double scalar) {
    return Tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w * scalar);
}

inline Tuple operator*(double scalar, const Tuple& t) { return t * scalar; }

inline Tuple operator/(const Tuple& t, double scalar) {
    return Tuple(t.x / scalar, t.y / scalar, t.z / scalar, t.w / scalar);
}

inline bool operator==(const Tuple& t1, const Tuple& t2) { 
    return (dbl_eql(t1.x, t2.x) && dbl_eql(t1.y, t2.y) && dbl_eql(t1.z, t2.z) &&
            dbl_eql(t1.w, t2.w));
}

inline double magnitude(const Tuple& t) {
    assert(t.isVector() && "Magnitude DNE for point");
    return ray_tracer::math::hypot(t);
}

inline Tuple norm(const Tuple& t) {
    assert(t.isVector() && "Cannot normalize a point");
    double mag = magnitude(t);
    return Tuple(t / mag);
}

inline double dot(const Tuple& t, const Tuple& v) {
    assert(t.isVector() && v.isVector() && "can only dot two vectors");
    return (t.x * v.x + t.y * v.y + t.z * v.z);
}

inline Tuple cross(const Tuple& t, const Tuple& v) {
    assert(t.isVector() && v.isVector() && "can only cross two vectors");
    return Tuple(t.y * v.z - t.z * v.y, t.z * v.x - t.x * v.z, t.x * v.y - t.y * v.x, 0);
}

Tuple reflect(const Tuple& in, const Tuple& normal);

}  // namespace ray_tracer::math
   // namespace ray_tracer::math