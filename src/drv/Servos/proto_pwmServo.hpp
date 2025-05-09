#pragma once
#include "MachTypes.hpp"

class ProtoServo {
public:
  virtual ~ProtoServo() {}

  virtual  void setAngle(const Angle& angle) = 0; // Set the servo angle
  virtual Angle getAngle() const = 0;             // Get the current servo angle

  virtual bool isEnabled() const = 0; // Check if servo is active (enabled)
  virtual void enable() = 0;          // Enable the servo (start sending PWM signal)
  virtual void disable() = 0;         // Disable the servo (stop PWM signal or set 0)
};
