#pragma once
#include <stdint.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846f
#endif

#define DEG_TO_RAD(x) ((x) * M_PI / 180.0f)
#define RAD_TO_DEG(x) ((x) * 180.0f / M_PI)


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
        constexpr Angle() : radians_(0.0f) {}

        constexpr Angle(Radians r) : radians_(r.value) {}

        constexpr Angle(Degrees d)
            : radians_(d.value * (M_PI / 180.0f)) {}

        constexpr float asRadians() const { return radians_; }

        constexpr float asDegrees() const {
            return radians_ * (180.0f / M_PI);
        }

        void setRadians(Radians r) { radians_ = r.value; }

        void setDegrees(Degrees d) {
            radians_ = d.value * (M_PI / 180.0f);
        }

        // Optional arithmetic operators
        Angle& operator+=(const Angle& other) {
            radians_ += other.radians_;
            return *this;
        }

        Angle operator+(const Angle& other) const {
            return Angle(Radians{radians_ + other.radians_});
        }

    private:
        float radians_;
    };
    