#pragma once
#include <stdint.h>

using Hertz = float;
using MicroSeconds = uint32_t;
using MilliSeconds = uint32_t;
using DutyCycle     = float;      // Range: 0.0 to 1.0
using Millimeters   = float;      // Distance in mm
using Newtons       = float;      // Force in N

#include <math.h>

inline DutyCycle clampDuty(DutyCycle dc) {
    return fminf(fmaxf(dc, 0.0f), 1.0f);
}