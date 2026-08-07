#pragma once

class Vector3D {
public:
    enum {
        X_AXIS = 0,
        Y_AXIS,
        Z_AXIS,
        Vec3Dim
    };
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[Vec3Dim];
    };
    Vector3D(
        float _x = 0.f,
        float _y = 0.f,
        float _z = 0.f
    );

    void print() const;
    void printXML() const;

    Vector3D  operator+(const  Vector3D& other) const;
    Vector3D& operator+=(const Vector3D& other);
    Vector3D  operator-(const  Vector3D& other) const;
    Vector3D& operator-=(const Vector3D& other);

    Vector3D  operator*(float  scalar) const;
    Vector3D& operator*=(float scalar);
    Vector3D  operator/(float  scalar) const;
    Vector3D& operator/=(float scalar);

    Vector3D  operator*(const  Vector3D& other) const;
    float     dot(  const Vector3D& other) const;
    Vector3D  cross(const Vector3D& other) const;

    float length() const;
    Vector3D normalize() const;
    bool isZero() const;

    Vector3D operator-() const { return Vector3D(-x, -y, -z); }

    Vector3D toRad() const;
    Vector3D toDeg() const;
};
typedef Vector3D* Vector3D_p;

#include <cmath>    // sqrtf()
static inline float len2D(float f1, float f2) {
    return sqrtf((f1*f1) + (f2*f2));
}
extern const Vector3D V0;
