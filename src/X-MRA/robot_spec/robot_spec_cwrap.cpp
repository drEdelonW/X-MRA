#include "robot_spec_cwrap.h"   // sync call prototype
#include "robot_spec.hpp"       // XMRA

void xArm() { XMRA.ARM(); }
void xDisarm() { XMRA.DISARM(); }