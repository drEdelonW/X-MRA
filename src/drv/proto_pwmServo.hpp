#pragma once
#include <stdint.h>

using Hertz = float;
using MicroSeconds = uint32_t;
using MilliSeconds = uint32_t;
using DutyCycle = float;        // Range: 0.0 to 1.0
using Tick = uint16_t;          // Controller-specific tick unit

class ProtoPWM {
  public:
    virtual ~ProtoPWM() {}

    virtual         void setPulseDurationUS(MicroSeconds duration_us) = 0;
    virtual MicroSeconds getPulseDurationUS() const = 0;

    virtual      void setDutyCycle(DutyCycle duty) = 0;
    virtual DutyCycle getDutyCycle() const = 0;

    virtual MicroSeconds getPeriodUS() const = 0;

    virtual void getRawTicks(Tick& on_tick, Tick& off_tick) const = 0;
    virtual void setRawTicks(Tick  on_tick, Tick  off_tick) = 0;

    virtual void disable() = 0;
    virtual void enable() = 0;
    virtual bool isEnabled() const = 0;
};
