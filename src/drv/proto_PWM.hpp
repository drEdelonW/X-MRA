#pragma once
#include <stdint.h>

#include "PhysTypes.hpp"

class ProtoPWM {
  public:
    virtual ~ProtoPWM() {}

    virtual MicroSeconds getPeriodUS() = 0;

    virtual      void setDutyCycle(DutyCycle duty) = 0;
    virtual DutyCycle getDutyCycle() = 0;

    virtual         void setDuration(MicroSeconds duration) = 0;
    virtual MicroSeconds getDuration() = 0;


    virtual void disable() = 0;
    virtual void enable() = 0;
    virtual bool isEnabled()  = 0;
};
