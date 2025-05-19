#pragma once
#include "JointBase.hpp"
#include "PhysTypes.hpp"
#include "Vector3d.hpp"
#include "Matrix4x4.hpp"

/*
      ▲ Z (up)
      │
      │    ▲ X (forward)
      │   ╱
      │  ╱
      │ ╱
      │╱
      ◉─────────► Y (right)
*/

/*  ArachnidLeg:

   [Body]
      |
   [Coxa]──▶──[Femur]
      |           |
   [Body]      [Tibia]
                  |
==============={Ground}==============

*/

class ArachnidLeg {
public:
   ArachnidLeg(
      JointBase& coxaJn,
      JointBase& femurJn,
      JointBase& tibiaJn,
      Millimeters offs = 0.0f,
      Angle rotation = deg(0.0F)
   );

   bool  checkJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   Vector3D   tipPosition(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   bool checkTipPosition(float x, float y, float z);
   bool checkTipPosition(Vector3D pos);

   void configureMount(Millimeters offset, Angle yaw);
   Vector3D bodyToLeg(Vector3D bodyPos);
   Vector3D legToBody(Vector3D legPos);


   bool applyPose();

   void activate();
   void deactivate();

private:
   JointBase& coxaJn_;
   JointBase& femurJn_;
   JointBase& tibiaJn_;

   const Millimeters coxaLength_  = (Millimeters) 27.0f;    // Length of coxa segment
   const Millimeters femurLength_ = (Millimeters) 85.1f;   // Length of femur segment
   const Millimeters tibiaLength_ = (Millimeters)144.23f;   // Length of tibia segment
   Matrix4x4 _bodyToLeg, _legToBody;
};
