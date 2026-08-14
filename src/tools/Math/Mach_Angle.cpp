#include "Mach_Angle.hpp"

// ----- constructors -----
Angle::Angle() : _aRadians(0.f) {}
Angle::Angle(Radians r) : _aRadians(r.rValue) {}
Angle::Angle(Degrees d) : _aRadians(d.dValue * (M_PI / 180.f)) {}

// ----- factories -----
Angle Angle::fromDegrees(float deg) { return Angle(Degrees{deg}); }
Angle Angle::fromRadians(float rad) { return Angle(Radians{rad}); }

// ----- conversions -----
float Angle::asRadians() const { return _aRadians; }
float Angle::asDegrees() const { return _aRadians * (180.f / M_PI); }

// ----- setters -----
void Angle::setRadians(Radians r) { _aRadians = r.rValue; }
void Angle::setDegrees(Degrees d) { _aRadians = d.dValue * (M_PI / 180.f); }

// ----- unary -----
Angle Angle::operator-() const { return Angle{ Radians(-_aRadians) }; }

Angle  Angle::operator+( const Angle& other) const  { return Angle(Radians{_aRadians + other._aRadians}); }
Angle& Angle::operator+=(const Angle& other)        { return *this = *this + other; }
Angle  Angle::operator-( const Angle& other) const  { return Angle(Radians{_aRadians - other._aRadians}); }
Angle& Angle::operator-=(const Angle& other)        { return *this = *this - other; }

// float scale
Angle  Angle::operator*( float scalar) const { return Angle::fromRadians(_aRadians * scalar); }
Angle& Angle::operator*=(float scalar)       { return *this = *this * scalar; }
Angle  Angle::operator/( float scalar) const { return Angle::fromRadians(_aRadians / scalar); }
Angle& Angle::operator/=(float scalar)       { return *this = *this / scalar; }

// int scale
Angle  Angle::operator*( int scalar) const   { return *this * static_cast<float>(scalar); }
Angle& Angle::operator*=(int scalar)         { return *this = *this * scalar; }
Angle  Angle::operator/( int scalar) const   { return *this / static_cast<float>(scalar); }
Angle& Angle::operator/=(int scalar)         { return *this = *this / scalar; }

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

// Angle Angle::clamp(const Angle& a, const Angle& min, const Angle& max) { return (a < min) ? min : (a > max ? max : a); }

// Angle Angle::min(const Angle& a, const Angle& b) { return (a < b) ? a : b; }
// Angle Angle::max(const Angle& a, const Angle& b) { return (a > b) ? a : b; }