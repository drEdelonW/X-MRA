#pragma once
#include "Phys_Lens.hpp"
#include "Phys_TimeFreq.hpp"
#include "Phys_Force.hpp"

#include <math.h>
using DutyCycle = float;    // Range: 0.0 to 1.0
typedef DutyCycle* DutyCycle_p;
inline DutyCycle GetClampDuty(DutyCycle dc) {
    return fminf(fmaxf(dc, 0.f), 1.f);
}

inline DutyCycle clampDuty(DutyCycle_p dc) {
    return *dc = GetClampDuty(*dc);
}
