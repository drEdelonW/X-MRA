#include "Matrix4x4.hpp"
#include <cmath>
#ifndef M_PI
#   define M_PI 3.14159265358979323846f
#endif

Matrix4x4::Matrix4x4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            data[i][j] = ((i == j) ?
                1.f : 0.f);
}

Matrix4x4::Matrix4x4(
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

Matrix4x4& Matrix4x4::reset() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            data[i][j] = ((i == j) ?
                1.f : 0.f);

    return *this;
}

Matrix4x4 Matrix4x4::createTranslation(Vector3D translation) {
    Matrix4x4 m;
    m.data[0][3] = translation.x;
    m.data[1][3] = translation.y;
    m.data[2][3] = translation.z;
    return m;
}

Matrix4x4 Matrix4x4::createScale(Vector3D& scale) {
    Matrix4x4 m;
    m.data[0][0] = scale.x;
    m.data[1][1] = scale.y;
    m.data[2][2] = scale.z;
    return m;
}

Matrix4x4 Matrix4x4::createRotationX(float angle) {
    Matrix4x4 m;
    float c = std::cos(angle);
    float s = std::sin(angle);
    m.data[1][1] = c; m.data[1][2] = -s;
    m.data[2][1] = s; m.data[2][2] = c;
    return m;
}

Matrix4x4 Matrix4x4::createRotationY(float angle) {
    Matrix4x4 m;
    float c = std::cos(angle);
    float s = std::sin(angle);
    m.data[0][0] = c;  m.data[0][2] = s;
    m.data[2][0] = -s; m.data[2][2] = c;
    return m;
}

Matrix4x4 Matrix4x4::createRotationZ(float angle) {
    Matrix4x4 m;
    float c = std::cos(angle);
    float s = std::sin(angle);
    m.data[0][0] = c; m.data[0][1] = -s;
    m.data[1][0] = s; m.data[1][1] = c;
    return m;
}

float Matrix4x4::degreesToRadians(float degrees) {
    return degrees * M_PI / 180.f;
}

Matrix4x4 Matrix4x4::createRotationXDegrees(float angleDegrees) {
    return createRotationX(
        degreesToRadians(angleDegrees)
    );
}

Matrix4x4 Matrix4x4::createRotationYDegrees(float angleDegrees) {
    return createRotationY(
        degreesToRadians(angleDegrees)
    );
}

Matrix4x4 Matrix4x4::createRotationZDegrees(float angleDegrees) {
    return createRotationZ(
        degreesToRadians(angleDegrees)
    );
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k)
                result.data[i][j] +=
                    data[i][k] * other.data[k][j];
        }
    return result;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
    return *this = *this * other;
}


Vector3D Matrix4x4::applyTransform(Vector3D& vec) const {
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
    if ((w != 1.0f) &&
        (w != 0.0f)
        )   result /= w;

    return result;
}

Vector3D Matrix4x4::applyTransform(const Vector3D& vec) const {
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
    if ((w != 1.0f) &&
        (w != 0.0f)
        )   result /= w;

    return result;
}