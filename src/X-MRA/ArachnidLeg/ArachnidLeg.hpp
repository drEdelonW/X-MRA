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

/*
       ▲ Z
       │
       │    ▲ X
       │   ╱
       │  ╱
       │ ╱
       │╱
       ◉─────────► Y
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

    const Millimeters coxaLength_ = 27.0f;    // Length of coxa segment
    const Millimeters femurLength_ = 85.1f;   // Length of femur segment
    const Millimeters tibiaLength_ = 139.2f;   // Length of tibia segment

    // Inverse kinematics calculation
    void calculateInverseKinematics(Millimeters x, Millimeters y, Millimeters z);
};
