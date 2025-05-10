#include "Mach_Angle.hpp"


Angle::Angle() : radians_(0.0f) {}
Angle::Angle(Radians r) : radians_(r.value) {}
Angle::Angle(Degrees d) : radians_(d.value * (M_PI / 180.0f)) {}

Angle Angle::fromDegrees(float deg) {
    return Angle(Degrees{deg});
}

Angle Angle::fromRadians(float rad) {
    return Angle(Radians{rad});
}

float Angle::asRadians() const {
    return radians_;
}

float Angle::asDegrees() const {
    return radians_ * (180.0f / M_PI);
}

void Angle::setRadians(Radians r) {
    radians_ = r.value;
}

void Angle::setDegrees(Degrees d) {
    radians_ = d.value * (M_PI / 180.0f);
}

Angle& Angle::operator+=(const Angle& other) {
    radians_ += other.radians_;
    return *this;
}

Angle& Angle::operator-=(const Angle& other) {
    radians_ -= other.radians_;
    return *this;
}

Angle Angle::operator+(const Angle& other) const {
    return Angle(Radians{radians_ + other.radians_});
}

Angle Angle::operator-(const Angle& other) const {
    return Angle(Radians{radians_ - other.radians_});
}

inline Angle Angle::operator*(float scalar) const {
    return Angle::fromRadians(radians_ * scalar);
}

inline Angle& Angle::operator*=(float scalar) {
    radians_ *= scalar;
    return *this;
}

inline Angle Angle::operator/(float scalar) const {
    return Angle::fromRadians(radians_ / scalar);
}

inline Angle& Angle::operator/=(float scalar) {
    radians_ /= scalar;
    return *this;
}

inline Angle Angle::operator*(int scalar) const {
    return *this * static_cast<float>(scalar);
}

inline Angle& Angle::operator*=(int scalar) {
    return *this *= static_cast<float>(scalar);
}

inline Angle Angle::operator/(int scalar) const {
    return *this / static_cast<float>(scalar);
}

inline Angle& Angle::operator/=(int scalar) {
    return *this /= static_cast<float>(scalar);
}


bool Angle::operator==(const Angle& other) const {
    return radians_ == other.radians_;
}

bool Angle::operator!=(const Angle& other) const {
    return !(*this == other);
}
