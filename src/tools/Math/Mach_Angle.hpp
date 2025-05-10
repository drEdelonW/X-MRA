#pragma once

#include <stdint.h>
#include "MachTypes.hpp"

#define deg(v)  Angle::fromDegrees(v)
#define rad(v)  Angle::fromRadians(v)

// Local fallback clamp
inline float clamp(float val, float minVal, float maxVal) {
  return (val < minVal) ? minVal : (val > maxVal ? maxVal : val);
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

    Angle  operator+(const Angle& other) const;
    Angle& operator+=(const Angle& other);

    Angle  operator-(const Angle& other) const;
    Angle& operator-=(const Angle& other);

    Angle operator*(float scalar) const;
    Angle& operator*=(float scalar);

    Angle operator/(float scalar) const;
    Angle& operator/=(float scalar);

    Angle operator*(int scalar) const;
    Angle& operator*=(int scalar);

    Angle operator/(int scalar) const;
    Angle& operator/=(int scalar);

    bool operator==(const Angle& other) const;
    bool operator!=(const Angle& other) const;

private:
    float radians_;
};
