#pragma once

#include "PhysTypes.hpp"
#include "JointBase.hpp"
#include "pwmServoJoint.hpp"

enum LegJoint {
   Coxa,
   Femur,
   Tibia,
   PhalNum
};

typedef struct {
//    JointBase_p jn;
   ServoJoint  jn;
   Millimeters len;
} Phalanx_t;
typedef Phalanx_t* Phalanx_p;
typedef Phalanx_p* Phalanx_ar;
