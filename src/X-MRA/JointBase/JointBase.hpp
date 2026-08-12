#pragma once

#include "Mach_Angle.hpp"
#include <cstdint>

class JointBase {
public:
    virtual ~JointBase() {}

    virtual bool  checkPose(const Angle& angle) = 0;
    virtual bool  applyPose() = 0;
    virtual Angle getAngle() const = 0;

    virtual void engage() = 0;
    virtual void release() = 0;
};
typedef JointBase* JointBase_p;
