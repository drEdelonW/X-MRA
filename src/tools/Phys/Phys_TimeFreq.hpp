#pragma once
using Hertz = float;

#include <stdint.h>
using MicroSeconds = uint64_t;
using MilliSeconds = uint64_t;
#include <time.h>
inline MicroSeconds microsNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return
        static_cast<MicroSeconds>(ts.tv_sec) * 1000000ULL +
        static_cast<MicroSeconds>(ts.tv_nsec) / 1000ULL;
}

inline MilliSeconds millisNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return
        static_cast<MilliSeconds>(ts.tv_sec) * 1000ULL +
        static_cast<MilliSeconds>(ts.tv_nsec) / 1000000ULL;
}
