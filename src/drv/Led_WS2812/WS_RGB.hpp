#pragma once

#include "types.h"

struct RGB {
    uint8_t r, g, b;
    constexpr RGB(
        uint8_t r = 0,
        uint8_t g = 0,
        uint8_t b = 0
    ) : r(r), g(g), b(b) {}
};

namespace Colors {
    constexpr RGB Black  (0,   0,   0);
    constexpr RGB White  (255, 255, 255);
    constexpr RGB Red    (255, 0,   0);
    constexpr RGB Green  (0,   255, 0);
    constexpr RGB Blue   (0,   0,   255);
    constexpr RGB Yellow (255, 255, 0);
    constexpr RGB Cyan   (0,   255, 255);
    constexpr RGB Magenta(255, 0,   255);
}

static inline uint8_t _scaleChannel(uint8_t v, float factor) {
    float scaled = v * factor;
    if (scaled <= 0.f)   return 0;
    if (scaled >= 255.f) return 255;
    return (uint8_t)(scaled + 0.5f);
}
static inline RGB operator*(RGB c, float factor) {
    return RGB(
        _scaleChannel(c.r, factor),
        _scaleChannel(c.g, factor),
        _scaleChannel(c.b, factor)
    );
}
inline RGB operator*(float factor, RGB c) { return c * factor; }

static inline RGB operator/(RGB c, float divisor) {
    return c * (1.f / divisor);
}

