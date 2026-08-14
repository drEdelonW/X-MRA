#pragma once

#include <stdint.h>
#include "MachTypes.hpp"

typedef enum : bool {
    CCW = false,    // CounterClockWise
    CW  = true      // ClockWise
} rotDir_t;

#define deg(v)  Angle::fromDegrees(v) /* set float as Degrees */
#define rad(v)  Angle::fromRadians(v) /* set float as Radians */

// Local fallback clamp
inline float Clamp(float minVal, float val, float maxVal) {
    return (val < minVal) ?
        minVal :
        ((val > maxVal) ?
            maxVal : val);
}

class Angle {
public:
    Angle();
    explicit Angle(Radians r);
    explicit Angle(Degrees d);

    static Angle fromDegrees(float deg);
    static Angle fromRadians(float rad);

    float asRadians() const;
    float asDegrees() const;

    void setRadians(Radians r);
    void setDegrees(Degrees d);

    Angle operator-() const;

    Angle  operator+( const Angle& other) const;
    Angle& operator+=(const Angle& other);
    Angle  operator-( const Angle& other) const;
    Angle& operator-=(const Angle& other);

    Angle  operator*( float scalar) const;
    Angle& operator*=(float scalar);
    Angle  operator/( float scalar) const;
    Angle& operator/=(float scalar);

    Angle  operator*( int scalar) const;
    Angle& operator*=(int scalar);
    Angle  operator/( int scalar) const;
    Angle& operator/=(int scalar);

    bool operator==(const Angle& other) const;
    bool operator!=(const Angle& other) const;
    bool operator< (const Angle& o) const;
    bool operator> (const Angle& o) const;
    bool operator<=(const Angle& o) const;
    bool operator>=(const Angle& o) const;

#if 0
    Angle  normalized() const;
    Angle& normalize();

    static Angle clamp(const Angle& a, const Angle& min, const Angle& max);
    static Angle min(const Angle& a, const Angle& b);
    static Angle max(const Angle& a, const Angle& b);

    static const Angle aZero;
    static const Angle Pi;
    static const Angle TwoPi;
#endif
private:
    float _aRadians;
};
typedef Angle* Angle_p;
