#pragma once

#include "Vector3d.hpp"

class M4x4 {
public:
    float data[4][4];

    M4x4();
    M4x4(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33
    );

    M4x4& reset();

    static M4x4 mxTrans(Vector3D translation);
    static M4x4 mxScale(Vector3D& scale);
    static M4x4 mxRotX(float angle);
    static M4x4 mxRotY(float angle);
    static M4x4 mxRotZ(float angle);
    static M4x4 mxRotDegX(float angleDegrees);
    static M4x4 mxRotDegY(float angleDegrees);
    static M4x4 mxRotDegZ(float angleDegrees);
    static float degreesToRadians(float degrees);

    M4x4  operator*( const M4x4& other) const;
    M4x4& operator*=(const M4x4& other);
    Vector3D applyTransform(Vector3D& vec) const;
    Vector3D applyTransform(const Vector3D& vec) const;
};
typedef M4x4* Matrix4x4_p;