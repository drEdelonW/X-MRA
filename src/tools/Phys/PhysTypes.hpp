#pragma once
#include "Phys_Lens.hpp"
#include "Phys_TimeFreq.hpp"
#include "Phys_Force.hpp"

#include <math.h>
using DutyCycle = float;    // Range: 0.0 to 1.0
inline DutyCycle clampDuty(DutyCycle dc) {
    return fminf(fmaxf(dc, 0.0f), 1.0f);
}
