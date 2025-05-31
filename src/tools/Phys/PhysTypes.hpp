#pragma once
#include <stdint.h>

using Hertz = float;
using MicroSeconds = uint64_t;
using MilliSeconds = uint64_t;
using DutyCycle     = float;      // Range: 0.0 to 1.0
using Millimeters   = float;      // Distance in mm
using Newtons       = float;      // Force in N

#include <math.h>

inline DutyCycle clampDuty(DutyCycle dc) {
    return fminf(fmaxf(dc, 0.0f), 1.0f);
}

#include <time.h>

inline MicroSeconds microsNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<MicroSeconds>(ts.tv_sec) * 1000000ULL + static_cast<MicroSeconds>(ts.tv_nsec) / 1000ULL;
}

inline MilliSeconds millisNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<MilliSeconds>(ts.tv_sec) * 1000ULL + static_cast<MilliSeconds>(ts.tv_nsec) / 1000000ULL;
}