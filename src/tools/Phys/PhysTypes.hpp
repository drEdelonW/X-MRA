#pragma once
#include "Phys_Lens.hpp"
#include "Phys_TimeFreq.hpp"
#include "Phys_Force.hpp"

#include <math.h>
using DutyCycle_t = float;    // Range: 0.0 to 1.0
typedef DutyCycle_t* DutyCycle_p;

inline DutyCycle_t GetClampDuty(DutyCycle_t dc) {
    return fminf(
        fmaxf(
            dc, 0.f
             ), 1.f
    );
}

inline DutyCycle_t clampDuty(DutyCycle_p dc) {
    return *dc = GetClampDuty(*dc);
}
