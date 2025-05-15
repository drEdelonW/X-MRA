#pragma once
#include "JointBase.hpp"
#include "PhysTypes.hpp"

/*  ArachnidLeg:

   [Body]
      |
   [Coxa]──▶──[Femur]
                  |
               [Tibia]
                  |
==============={Ground}==============

*/

class ArachnidLeg {
public:
    ArachnidLeg(JointBase& coxaJn, JointBase& femurJn, JointBase& tibiaJn);

    void setJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);   // Set joint angles directly
    void setTipPosition(float x, float y, float z);             // Set leg tip position using inverse kinematics

    void activate();
    void deactivate();

private:
    JointBase& coxaJn_;
    JointBase& femurJn_;
    JointBase& tibiaJn_;

    const Millimeters coxaLength_ = 4.0f;    // Length of coxa segment
    const Millimeters femurLength_ = 3.5f;   // Length of femur segment
    const Millimeters tibiaLength_ = 8.7f;   // Length of tibia segment

    // Inverse kinematics calculation
    void calculateInverseKinematics(Millimeters x, Millimeters y, Millimeters z);
};
