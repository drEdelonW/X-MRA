#pragma once

#include "MachTypes.hpp"
#include <cstdint>

class JointBase {
public:
    virtual ~JointBase() {}

    virtual void setAngle(const Angle& angle) = 0;
    virtual Angle getAngle() const = 0;

    virtual void setForceLimit(float newtonLimit) {}
    virtual float getForceLimit() const { return -1.0f; }

    // virtual float getTorqueEstimate() const { return 0.0f; }

    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool isEnabled() const = 0;
};
