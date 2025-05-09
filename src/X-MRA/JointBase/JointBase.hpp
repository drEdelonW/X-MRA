#pragma once

#include "MachTypes.hpp"
#include <cstdint>

// Base class for any joint: rotational or linear actuator
class JointBase {
public:
    virtual ~JointBase() {}

    // Set the target angle or position for the joint
    virtual void setAngle(const Angle& angle) = 0;

    // Get the current or last commanded angle
    virtual Angle getAngle() const = 0;

    // Optional: set maximum allowed force/torque (for smart servos or physics simulators)
    virtual void setForceLimit(float newtonLimit) {}         // default: no-op
    virtual float getForceLimit() const { return -1.0f; }    // default: unsupported

    // Optional: get estimated torque or load from sensor (if available)
    virtual float getTorqueEstimate() const { return 0.0f; } // default: zero

    // Enable the joint (activate output or control loop)
    virtual void enable() = 0;

    // Disable the joint (turn off actuator output)
    virtual void disable() = 0;

    // Query whether the joint is enabled
    virtual bool isEnabled() const = 0;
};
