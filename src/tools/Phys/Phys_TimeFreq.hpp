#pragma once
using Hertz = float;

#include <stdint.h>
#include <time.h>

/* 1 second = 1'000 MilliSeconds = 1'000'000 MicroSeconds */
#define us(v)   MicroSeconds(v) /* Microseconds shortcut */
struct MicroSeconds { /* Duration/timestamp in microseconds */
    explicit constexpr MicroSeconds(uint64_t v) : usValue(v) {}
    uint64_t usValue;

    constexpr MicroSeconds operator+(MicroSeconds o) const { return MicroSeconds(usValue + o.usValue); }
    constexpr MicroSeconds operator-(MicroSeconds o) const { return MicroSeconds(usValue - o.usValue); }
    constexpr MicroSeconds operator*(float scale)    const { return MicroSeconds(static_cast<uint64_t>(usValue * scale)); }

    constexpr bool operator< (MicroSeconds o) const { return usValue <  o.usValue; }
    constexpr bool operator<=(MicroSeconds o) const { return usValue <= o.usValue; }
    constexpr bool operator> (MicroSeconds o) const { return usValue >  o.usValue; }
    constexpr bool operator>=(MicroSeconds o) const { return usValue >= o.usValue; }
    constexpr bool operator==(MicroSeconds o) const { return usValue == o.usValue; }
    constexpr bool operator!=(MicroSeconds o) const { return usValue != o.usValue; }
};
constexpr MicroSeconds  operator*(float scale, MicroSeconds v)      { return v * scale; }
constexpr float         operator/(MicroSeconds a, MicroSeconds b)   { return static_cast<float>(a.usValue) / static_cast<float>(b.usValue); }
constexpr MicroSeconds  operator/(MicroSeconds a, float scale)      { return MicroSeconds(static_cast<uint64_t>(a.usValue / scale)); }
constexpr MicroSeconds  operator/(MicroSeconds a, int   scale)      { return MicroSeconds(a.usValue / static_cast<uint64_t>(scale)); }

inline MicroSeconds microsNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return MicroSeconds(
        static_cast<uint64_t>(ts.tv_sec) * 1000000ULL +
        static_cast<uint64_t>(ts.tv_nsec) / 1000ULL
    );
}

/* 1 second = 1'000 MilliSeconds = 1'000'000 MicroSeconds */
#define ms(v)   MilliSeconds(v) /* Milliseconds shortcut */
struct MilliSeconds { /* Duration/timestamp in milliseconds */
    explicit constexpr MilliSeconds(uint64_t v) : msValue(v) {}
    uint64_t msValue;

    constexpr MilliSeconds operator+(MilliSeconds o) const { return MilliSeconds(msValue + o.msValue); }
    constexpr MilliSeconds operator-(MilliSeconds o) const { return MilliSeconds(msValue - o.msValue); }
    constexpr MilliSeconds operator*(float scale)    const { return MilliSeconds(static_cast<uint64_t>(msValue * scale)); }

    constexpr bool operator< (MilliSeconds o) const { return msValue <  o.msValue; }
    constexpr bool operator<=(MilliSeconds o) const { return msValue <= o.msValue; }
    constexpr bool operator> (MilliSeconds o) const { return msValue >  o.msValue; }
    constexpr bool operator>=(MilliSeconds o) const { return msValue >= o.msValue; }
    constexpr bool operator==(MilliSeconds o) const { return msValue == o.msValue; }
    constexpr bool operator!=(MilliSeconds o) const { return msValue != o.msValue; }
};
constexpr MilliSeconds  operator*(float scale, MilliSeconds v)      { return v * scale; }
constexpr float         operator/(MilliSeconds a, MilliSeconds b)   { return static_cast<float>(a.msValue) / static_cast<float>(b.msValue); }
constexpr MilliSeconds  operator/(MilliSeconds a, float scale)      { return MilliSeconds(static_cast<uint64_t>(a.msValue / scale)); }
constexpr MilliSeconds  operator/(MilliSeconds a, int   scale)      { return MilliSeconds(a.msValue / static_cast<uint64_t>(scale)); }

inline MilliSeconds millisNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return MilliSeconds(
        static_cast<uint64_t>(ts.tv_sec) * 1000ULL +
        static_cast<uint64_t>(ts.tv_nsec) / 1000000ULL
    );
}

// cross-type conversion — free functions, both structs already complete here
constexpr MicroSeconds MsToUs(MilliSeconds ms) { return MicroSeconds(ms.msValue * 1000ULL); }
constexpr MilliSeconds UsToMs(MicroSeconds us) { return MilliSeconds(us.usValue / 1000ULL); }