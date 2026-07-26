#pragma once
#include "Mach_Angle.hpp"

class ProtoServo {
public:
  virtual ~ProtoServo() {}

  virtual void  setAngle(const Angle& angle) = 0; // Set the servo angle
  virtual Angle getAngle() const = 0;             // Get the current servo angle

  virtual void  setAngleOffset(const Angle& angle) = 0; // Set the servo angle base Offset
  virtual Angle getAngleOffset() const = 0;             // Get the current servo angle base Offset

  virtual void setReversion(const rotDir_t isReversed) = 0;

  virtual bool isEnabled() const = 0; // Check if servo is active (enabled)
  virtual void enable() = 0;          // Enable the servo (start sending PWM signal)
  virtual void disable() = 0;         // Disable the servo (stop PWM signal or set 0)
};
