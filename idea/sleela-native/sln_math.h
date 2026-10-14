/*
 * idea/sleela-native/sln_math.h
 * Sleela-Native -- original 3D math (vectors, 4x4 matrices, camera transforms).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Clean-room, original code. No third-party source. Header-only, dependency-free
 * (only <cmath>). Column-major 4x4 matrices to match OpenGL's convention.
 *
 * SPDX-License-Identifier: MIT  (see PROVENANCE.md)
 */
#ifndef SLN_MATH_H
#define SLN_MATH_H

#include <cmath>

namespace sln {

struct Vec3 {
    float x = 0, y = 0, z = 0;
    Vec3() = default;
    Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

    Vec3 operator+(const Vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec3 operator-(const Vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec3 operator*(float s)       const { return {x * s, y * s, z * s}; }
};

inline float dot(const Vec3 &a, const Vec3 &b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
inline Vec3  cross(const Vec3 &a, const Vec3 &b) {
    return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}
inline float length(const Vec3 &v) { return std::sqrt(dot(v, v)); }
inline Vec3  normalize(const Vec3 &v) {
    float l = length(v);
    return (l > 1e-8f) ? v * (1.0f / l) : v;
}

/* Column-major 4x4 matrix: m[col*4 + row], ready to upload to GL. */
struct Mat4 {
    float m[16] = {0};

    static Mat4 identity() {
        Mat4 r; r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f; return r;
    }

    Mat4 operator*(const Mat4 &b) const {
        Mat4 r;
        for (int c = 0; c < 4; ++c)
            for (int row = 0; row < 4; ++row) {
                float s = 0;
                for (int k = 0; k < 4; ++k) s += m[k*4 + row] * b.m[c*4 + k];
                r.m[c*4 + row] = s;
            }
        return r;
    }
};

/* Right-handed look-at view matrix. */
inline Mat4 look_at(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
    Vec3 f = normalize(center - eye);
    Vec3 s = normalize(cross(f, up));
    Vec3 u = cross(s, f);
    Mat4 r = Mat4::identity();
    r.m[0] = s.x;  r.m[4] = s.y;  r.m[8]  = s.z;
    r.m[1] = u.x;  r.m[5] = u.y;  r.m[9]  = u.z;
    r.m[2] = -f.x; r.m[6] = -f.y; r.m[10] = -f.z;
    r.m[12] = -dot(s, eye);
    r.m[13] = -dot(u, eye);
    r.m[14] =  dot(f, eye);
    return r;
}

/* Right-handed perspective projection (GL clip space, [-1,1] depth). */
inline Mat4 perspective(float fovy_rad, float aspect, float znear, float zfar) {
    float t = std::tan(fovy_rad * 0.5f);
    Mat4 r; /* zero-initialised */
    r.m[0]  = 1.0f / (aspect * t);
    r.m[5]  = 1.0f / t;
    r.m[10] = -(zfar + znear) / (zfar - znear);
    r.m[11] = -1.0f;
    r.m[14] = -(2.0f * zfar * znear) / (zfar - znear);
    return r;
}

/* Rotation about the Y axis (for the orbiting model / light). */
inline Mat4 rotate_y(float a) {
    Mat4 r = Mat4::identity();
    float c = std::cos(a), s = std::sin(a);
    r.m[0] = c; r.m[2] = s; r.m[8] = -s; r.m[10] = c;
    return r;
}

inline Mat4 translate(const Vec3 &t) {
    Mat4 r = Mat4::identity();
    r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
    return r;
}

} // namespace sln

#endif /* SLN_MATH_H */
