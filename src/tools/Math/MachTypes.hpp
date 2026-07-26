#pragma once

#include <stdint.h>
#include <cmath>

#ifndef M_PI
  #define M_PI 3.14159265358979323846f
#endif

struct Degrees {
    explicit constexpr Degrees(float v) : dValue(v) {}
    float dValue;
};

struct Radians {
    explicit constexpr Radians(float v) : rValue(v) {}
    float rValue;
};
