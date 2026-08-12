#pragma once
#include "Matrix4x4.hpp" // #include "Vector3d.hpp"
#include "Phalanx.hpp"

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

#define JFREEZE  rad(NAN)

class ArachnidLeg {
public:
#if 0
   ArachnidLeg(
      JointBase*  Jn,
      // ServoJoint* Jn,
      Millimeters offs = 0.f,
      Angle       rotation = deg(0.f)
   );
   ArachnidLeg(
      // JointBase*  Jn,
      ServoJoint* Jn,
      Millimeters offs = 0.f,
      Angle       rotation = deg(0.f)
   );
#else
   ArachnidLeg(
      Phalanx_p   Phalanx,
      Millimeters offs = 0.f,
      Angle       rotation = deg(0.f)
   );
#endif

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
   Phalanx_t _Phalanx[PhalNum];

   void _configMount(Millimeters offset, Angle yaw);

   bool _checkTipPosLegSpace(Vector3D pos);
   bool _checkTipPosLegSpace(float x, float y, float z);

   M4x4 _bodyToLeg;
   Vector3D bodyToLeg(Vector3D bodyPos);

   M4x4 _legToBody;
   Vector3D legToBody(Vector3D legPos);
};
typedef ArachnidLeg* ArachnidLeg_p;
