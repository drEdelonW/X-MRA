#include "Mach_Angle.hpp"

// ----- constructors -----
Angle::Angle() : radians_(0.0f) {}
Angle::Angle(Radians r) : radians_(r.value) {}
Angle::Angle(Degrees d) : radians_(d.value * (M_PI / 180.0f)) {}

// ----- factories -----
Angle Angle::fromDegrees(float deg) { return Angle(Degrees{deg}); }
Angle Angle::fromRadians(float rad) { return Angle(Radians{rad}); }

// ----- conversions -----
float Angle::asRadians() const { return radians_; }
float Angle::asDegrees() const { return radians_ * (180.0f / M_PI); }

// ----- setters -----
void Angle::setRadians(Radians r) { radians_ = r.value; }
void Angle::setDegrees(Degrees d) { radians_ = d.value * (M_PI / 180.0f); }

// ----- unary -----
Angle Angle::operator-() const { return Angle{ Radians(-radians_) }; }

// // ----- arithmetic -----
// #define ANGLE_ARITH(op)                     \
//     Angle Angle::operator op(const Angle& other) const {           \
//         return Angle{ Radians(radians_ op other.radians_) };       \
//     }                                                          \
//     Angle& Angle::operator op##=(const Angle& other) {             \
//         radians_ op##= other.radians_; return *this;               \
//     }

// ANGLE_ARITH(+)
// ANGLE_ARITH(-)
// #undef ANGLE_ARITH

Angle  Angle::operator+(const Angle& other) const   { return Angle(Radians{radians_ + other.radians_}); }
Angle  Angle::operator-(const Angle& other) const   { return Angle(Radians{radians_ - other.radians_}); }
Angle& Angle::operator+=(const Angle& other)        { radians_ += other.radians_; return *this; }
Angle& Angle::operator-=(const Angle& other)        { radians_ -= other.radians_; return *this; }

// float scale
inline Angle  Angle::operator*(float scalar) const  { return Angle::fromRadians(radians_ * scalar); }
inline Angle& Angle::operator*=(float scalar)       { radians_ *= scalar; return *this; }
inline Angle  Angle::operator/(float scalar) const  { return Angle::fromRadians(radians_ / scalar); }
inline Angle& Angle::operator/=(float scalar)       { radians_ /= scalar; return *this; }

// int scale
inline Angle  Angle::operator*(int scalar) const    { return *this * static_cast<float>(scalar); }
inline Angle& Angle::operator*=(int scalar)         { return *this *= static_cast<float>(scalar); }
inline Angle  Angle::operator/(int scalar) const    { return *this / static_cast<float>(scalar); }
inline Angle& Angle::operator/=(int scalar)         { return *this /= static_cast<float>(scalar); }

// ----- comparisons -----
bool Angle::operator==(const Angle& other) const { return radians_ == other.radians_; }
bool Angle::operator!=(const Angle& other) const { return !(*this == other); }
bool Angle::operator< (const Angle& other) const { return radians_ <  other.radians_; }
bool Angle::operator> (const Angle& other) const { return radians_ >  other.radians_; }
bool Angle::operator<=(const Angle& other) const { return radians_ <= other.radians_; }
bool Angle::operator>=(const Angle& other) const { return radians_ >= other.radians_; }

// // ----- helpers -----
// Angle Angle::normalized() const { return Angle{ Radians(wrapPi(radians_)) }; }
// Angle& Angle::normalize()       { radians_ = wrapPi(radians_); return *this; }

// Angle Angle::clamp(const Angle& a, const Angle& min, const Angle& max)
// {
//     return (a < min) ? min : (a > max ? max : a);
// }

// Angle Angle::min(const Angle& a, const Angle& b) { return (a < b) ? a : b; }
// Angle Angle::max(const Angle& a, const Angle& b) { return (a > b) ? a : b; }