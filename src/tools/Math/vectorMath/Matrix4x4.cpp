#include "Matrix4x4.hpp"
#include <cmath>
#ifndef M_PI
#   define M_PI 3.14159265358979323846f
#endif

M4x4::M4x4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            data[i][j] = ((i == j) ?
                1.f : 0.f);
}

M4x4::M4x4(
    float a00, float a01, float a02, float a03,
    float a10, float a11, float a12, float a13,
    float a20, float a21, float a22, float a23,
    float a30, float a31, float a32, float a33
) {
    data[0][0] = a00; data[0][1] = a01; data[0][2] = a02; data[0][3] = a03;
    data[1][0] = a10; data[1][1] = a11; data[1][2] = a12; data[1][3] = a13;
    data[2][0] = a20; data[2][1] = a21; data[2][2] = a22; data[2][3] = a23;
    data[3][0] = a30; data[3][1] = a31; data[3][2] = a32; data[3][3] = a33;
}

M4x4& M4x4::reset() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            data[i][j] = ((i == j) ?
                1.f : 0.f);

    return *this;
}

M4x4 M4x4::mxTrans(Vector3D translation) {
    M4x4 m;
    m.data[0][3] = translation.x;
    m.data[1][3] = translation.y;
    m.data[2][3] = translation.z;
    return m;
}

M4x4 M4x4::mxScale(Vector3D& scale) {
    M4x4 m;
    m.data[0][0] = scale.x;
    m.data[1][1] = scale.y;
    m.data[2][2] = scale.z;
    return m;
}

M4x4 M4x4::mxRotX(float angle) {
    M4x4 m;
    float c = std::cos(angle);
    float s = std::sin(angle);
    m.data[1][1] = c; m.data[1][2] = -s;
    m.data[2][1] = s; m.data[2][2] = c;
    return m;
}

M4x4 M4x4::mxRotY(float angle) {
    M4x4 m;
    float c = std::cos(angle);
    float s = std::sin(angle);
    m.data[0][0] = c;  m.data[0][2] = s;
    m.data[2][0] = -s; m.data[2][2] = c;
    return m;
}

M4x4 M4x4::mxRotZ(float angle) {
    M4x4 m;
    float c = std::cos(angle);
    float s = std::sin(angle);
    m.data[0][0] = c; m.data[0][1] = -s;
    m.data[1][0] = s; m.data[1][1] = c;
    return m;
}

float M4x4::degreesToRadians(float degrees) {
    return degrees * M_PI / 180.f;
}

M4x4 M4x4::mxRotDegX(float angleDegrees) {
    return mxRotX(
        degreesToRadians(angleDegrees)
    );
}

M4x4 M4x4::mxRotDegY(float angleDegrees) {
    return mxRotY(
        degreesToRadians(angleDegrees)
    );
}

M4x4 M4x4::mxRotDegZ(float angleDegrees) {
    return mxRotZ(
        degreesToRadians(angleDegrees)
    );
}

M4x4 M4x4::operator*(const M4x4& other) const {
    M4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = 0.f;
            for (int k = 0; k < 4; ++k)
                result.data[i][j] +=
                    data[i][k] * other.data[k][j];
        }
    return result;
}

M4x4& M4x4::operator*=(const M4x4& other) {
    return *this = *this * other;
}


Vector3D M4x4::applyTransform(Vector3D& vec) const {
    Vector3D result = (
        (data[0][0] * vec.x) + (data[0][1] * vec.y) +
        (data[0][2] * vec.z) + data[0][3],
        (data[1][0] * vec.x) + (data[1][1] * vec.y) +
        (data[1][2] * vec.z) + data[1][3],
        (data[2][0] * vec.x) + (data[2][1] * vec.y) +
        (data[2][2] * vec.z) + data[2][3]
        );
    float w =
        (data[3][0] * vec.x) + (data[3][1] * vec.y) +
        (data[3][2] * vec.z) + (data[3][3]);
    if ((w != 1.f) &&
        (w != 0.f)
        )   result /= w;

    return result;
}

Vector3D M4x4::applyTransform(const Vector3D& vec) const {
    Vector3D result = (
        (data[0][0] * vec.x) + (data[0][1] * vec.y) +
        (data[0][2] * vec.z) + data[0][3],
        (data[1][0] * vec.x) + (data[1][1] * vec.y) +
        (data[1][2] * vec.z) + data[1][3],
        (data[2][0] * vec.x) + (data[2][1] * vec.y) +
        (data[2][2] * vec.z) + data[2][3]
        );
    float w =
        (data[3][0] * vec.x) + (data[3][1] * vec.y) +
        (data[3][2] * vec.z) + (data[3][3]);
    if ((w != 1.f) &&
        (w != 0.f)
        )   result /= w;

    return result;
}