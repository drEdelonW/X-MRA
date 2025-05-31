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

#define JFREEZE    rad(NAN)

class ArachnidLeg {
public:
   ArachnidLeg(
      JointBase& coxaJn,
      JointBase& femurJn,
      JointBase& tibiaJn,
      Millimeters offs = 0.0f,
      Angle rotation = deg(0.0F)
   );

   void configMount(Millimeters offset, Angle yaw);

   bool  checkJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   bool    tryJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   // Angle getJointAngles(); // TODO: to design and implement.

   Vector3D    tipPosLegSpace(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   Vector3D   tipPosBodySpace(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   bool checkTipPosBodySpace(Vector3D bodyPos);
   bool checkTipPosLegSpace(Vector3D legPos);

   bool applyPose();

   void engage();
   void release();

private:

   Vector3D bodyToLeg(Vector3D bodyPos);
   Vector3D legToBody(Vector3D legPos);

   bool _checkTipPosLegSpace(float x, float y, float z);

   JointBase& coxaJn_;
   JointBase& femurJn_;
   JointBase& tibiaJn_;

   const Millimeters coxaLength_  = (Millimeters) 27.0f;    // Length of coxa segment
   const Millimeters femurLength_ = (Millimeters) 85.1f;    // Length of femur segment
   const Millimeters tibiaLength_ = (Millimeters)144.23f;   // Length of tibia segment
   Matrix4x4 _bodyToLeg, _legToBody;
};
