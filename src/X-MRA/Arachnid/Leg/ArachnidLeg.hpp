#pragma once
#include "Matrix4x4.hpp" // #include "Vector3d.hpp"
#include "PhysTypes.hpp"
#include "JointBase.hpp"


/*  ArachnidLeg:   /|
                  / |
   [Body]    [Femur]|
      |        /    |
      |-[Coxa]/     |
      |          [Tibia]
   [Body]           |
                    |
==============={Ground}==============
*/

#define JFREEZE    rad(NAN)

class ArachnidLeg {
public:
   enum LegJoint {
      COXA,
      FEMUR,
      TIBIA
   };
   ArachnidLeg(
      JointBase& coxaJn,
      JointBase& femurJn,
      JointBase& tibiaJn,
      Millimeters offs = 0.f,
      Angle rotation = deg(0.f)
   );

   void configMount(Millimeters offset, Angle yaw);

   bool  checkJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   bool    tryJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);

   Angle getJointAngles(LegJoint jName);

   Vector3D   tipPosLegSpace (Angle coxaAng, Angle femurAng, Angle tibiaAng);
   Vector3D   tipPosBodySpace(Angle coxaAng, Angle femurAng, Angle tibiaAng);
   bool  checkTipPosLegSpace (Vector3D legPos);
   bool  checkTipPosBodySpace(Vector3D bodyPos);

   bool applyPose();

   void engage();
   void release();

private:
   Vector3D bodyToLeg(Vector3D bodyPos);
   Vector3D legToBody(Vector3D legPos);

   bool _checkTipPosLegSpace(float x, float y, float z);
   bool _checkTipPosLegSpace(Vector3D pos);

   JointBase& _jnCoxa;
   JointBase& _jnFemur;
   JointBase& _jnTibia;

   const Millimeters coxaLength_  = mm( 27.0f );
   const Millimeters femurLength_ = mm( 85.1f );
   const Millimeters tibiaLength_ = mm(144.23f);
   Matrix4x4 _bodyToLeg, _legToBody;
};
