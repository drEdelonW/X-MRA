// Vector3D.cpp
#include "Vector3d.hpp"
#include <cmath>
#include <limits>

#ifndef M_PI
#   define M_PI 3.14159265358979323846f
#endif

const Vector3D V0 = {0.f, 0.f, 0.f};
Vector3D::Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3D& Vector3D::operator+=(const Vector3D& other) { return *this = *this + other; }
Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(
        (x + other.x),
        (y + other.y),
        (z + other.z)
    );
}

Vector3D& Vector3D::operator-=(const Vector3D& other) { return *this = *this - other; }
Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(
        (x - other.x),
        (y - other.y),
        (z - other.z)
    );
}

Vector3D& Vector3D::operator*=(float scalar) { return *this = *this * scalar; }
Vector3D Vector3D::operator*(float scalar) const {
    return Vector3D(
        (x * scalar),
        (y * scalar),
        (z * scalar)
    );
}
Vector3D Vector3D::operator*(const Vector3D& other) const { return cross(other); }

Vector3D& Vector3D::operator/=(float scalar) { return *this = *this / scalar; }
Vector3D Vector3D::operator/(float scalar) const {
    if (scalar == 0.f)
        return Vector3D(
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN()
        );
    else
        return Vector3D(
            (x / scalar),
            (y / scalar),
            (z / scalar)
        );

}

float Vector3D::dot(const Vector3D& other) const {
    return
        (x * other.x) +
        (y * other.y) +
        (z * other.z);
}
Vector3D Vector3D::cross(const Vector3D& other) const {
    Vector3D n{
        (y * other.z) - (z * other.y),
        (z * other.x) - (x * other.z),
        (x * other.y) - (y * other.x)
    };
    if (n.isZero())
        return Vector3D(
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN()
        );
    else
        return n;
}

Vector3D Vector3D::normalize() const { return *this / length(); }
float Vector3D::length() const {
    return
        std::sqrtf(
            (x*x) +
            (y*y) +
            (z*z)
        );
}

bool Vector3D::isZero() const {
    return
        (x == 0.f) &&
        (y == 0.f) &&
        (z == 0.f);
}

Vector3D Vector3D::toRad() const { return *this * (M_PI / 180.f); }
Vector3D Vector3D::toDeg() const { return *this * (180.f / M_PI); }

#include "terminal_tools.h" // LOG()
void Vector3D::print()    const { LOG("Vector3D(x: %+.3f, y: %+.3f, z: %+.3f)", x, y, z); }
void Vector3D::printXML() const { LOG("<Vector3D x=\"%f\" y=\"%f\" z=\"%f\" />\n", x, y, z); }
