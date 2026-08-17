#pragma once

#include "PhysTypes.hpp"
#include "JointBase.hpp"
#include "pwmServoJoint.hpp"

typedef struct {
//    JointBase_p jn; // TODO: use abstract type
   ServoJoint  jn;
   Millimeters len;
} Phalanx_t;
typedef Phalanx_t* Phalanx_p;
typedef Phalanx_p* Phalanx_ar;
