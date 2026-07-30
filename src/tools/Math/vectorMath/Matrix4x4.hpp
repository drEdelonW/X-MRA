#pragma once

#include "Vector3d.hpp"

class Matrix4x4 {
public:
    float data[4][4];

    Matrix4x4();
    Matrix4x4(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33
    );

    Matrix4x4& reset();

    static Matrix4x4 createTranslation(Vector3D translation);
    static Matrix4x4 createScale(Vector3D& scale);
    static Matrix4x4 createRotationX(float angle);
    static Matrix4x4 createRotationY(float angle);
    static Matrix4x4 createRotationZ(float angle);
    static Matrix4x4 createRotationXDegrees(float angleDegrees);
    static Matrix4x4 createRotationYDegrees(float angleDegrees);
    static Matrix4x4 createRotationZDegrees(float angleDegrees);
    static float degreesToRadians(float degrees);

    Matrix4x4 operator*(const Matrix4x4& other) const;
    Matrix4x4& operator*=(const Matrix4x4& other);
    Vector3D applyTransform(Vector3D& vec) const;
    Vector3D applyTransform(const Vector3D& vec) const;
};
typedef Matrix4x4* Matrix4x4_p;