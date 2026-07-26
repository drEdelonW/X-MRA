#include "Mach_Angle.hpp"

// ----- constructors -----
Angle::Angle() : _aRadians(0.0f) {}
Angle::Angle(Radians r) : _aRadians(r.rValue) {}
Angle::Angle(Degrees d) : _aRadians(d.dValue * (M_PI / 180.0f)) {}

// ----- factories -----
Angle Angle::fromDegrees(float deg) { return Angle(Degrees{deg}); }
Angle Angle::fromRadians(float rad) { return Angle(Radians{rad}); }

// ----- conversions -----
float Angle::asRadians() const { return _aRadians; }
float Angle::asDegrees() const { return _aRadians * (180.0f / M_PI); }

// ----- setters -----
void Angle::setRadians(Radians r) { _aRadians = r.rValue; }
void Angle::setDegrees(Degrees d) { _aRadians = d.dValue * (M_PI / 180.0f); }

// ----- unary -----
Angle Angle::operator-() const { return Angle{ Radians(-_aRadians) }; }

// // ----- arithmetic -----
// #define ANGLE_ARITH(op)                     \
//     Angle Angle::operator op(const Angle& other) const {           \
//         return Angle{ Radians(_aRadians op other._aRadians) };       \
//     }                                                          \
//     Angle& Angle::operator op##=(const Angle& other) {             \
//         _aRadians op##= other._aRadians; return *this;               \
//     }

// ANGLE_ARITH(+)
// ANGLE_ARITH(-)
// #undef ANGLE_ARITH

Angle  Angle::operator+(const Angle& other) const   { return Angle(Radians{_aRadians + other._aRadians}); }
Angle  Angle::operator-(const Angle& other) const   { return Angle(Radians{_aRadians - other._aRadians}); }
Angle& Angle::operator+=(const Angle& other)        { _aRadians += other._aRadians; return *this; }
Angle& Angle::operator-=(const Angle& other)        { _aRadians -= other._aRadians; return *this; }

// float scale
inline Angle  Angle::operator*(float scalar) const  { return Angle::fromRadians(_aRadians * scalar); }
inline Angle& Angle::operator*=(float scalar)       { _aRadians *= scalar; return *this; }
inline Angle  Angle::operator/(float scalar) const  { return Angle::fromRadians(_aRadians / scalar); }
inline Angle& Angle::operator/=(float scalar)       { _aRadians /= scalar; return *this; }

// int scale
inline Angle  Angle::operator*(int scalar) const    { return *this * static_cast<float>(scalar); }
inline Angle& Angle::operator*=(int scalar)         { return *this *= static_cast<float>(scalar); }
inline Angle  Angle::operator/(int scalar) const    { return *this / static_cast<float>(scalar); }
inline Angle& Angle::operator/=(int scalar)         { return *this /= static_cast<float>(scalar); }

// ----- comparisons -----
bool Angle::operator==(const Angle& other) const { return _aRadians == other._aRadians; }
bool Angle::operator!=(const Angle& other) const { return !(*this == other); }
bool Angle::operator< (const Angle& other) const { return _aRadians <  other._aRadians; }
bool Angle::operator> (const Angle& other) const { return _aRadians >  other._aRadians; }
bool Angle::operator<=(const Angle& other) const { return _aRadians <= other._aRadians; }
bool Angle::operator>=(const Angle& other) const { return _aRadians >= other._aRadians; }

// // ----- helpers -----
// Angle Angle::normalized() const { return Angle{ Radians(wrapPi(_aRadians)) }; }
// Angle& Angle::normalize()       { _aRadians = wrapPi(_aRadians); return *this; }

// Angle Angle::clamp(const Angle& a, const Angle& min, const Angle& max)
// {
//     return (a < min) ? min : (a > max ? max : a);
// }

// Angle Angle::min(const Angle& a, const Angle& b) { return (a < b) ? a : b; }
// Angle Angle::max(const Angle& a, const Angle& b) { return (a > b) ? a : b; }