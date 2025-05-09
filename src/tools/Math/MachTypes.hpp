#pragma once

#include <stdint.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846f
#endif

// Local fallback clamp
inline float clamp(float val, float minVal, float maxVal) {
  return (val < minVal) ? minVal : (val > maxVal ? maxVal : val);
}

struct Degrees {
    explicit constexpr Degrees(float v) : value(v) {}
    float value;
};

struct Radians {
    explicit constexpr Radians(float v) : value(v) {}
    float value;
};

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

    Angle& operator+=(const Angle& other);
    Angle& operator-=(const Angle& other);
    Angle  operator+(const Angle& other) const;
    Angle  operator-(const Angle& other) const;

    bool operator==(const Angle& other) const;
    bool operator!=(const Angle& other) const;

private:
    float radians_;
};
