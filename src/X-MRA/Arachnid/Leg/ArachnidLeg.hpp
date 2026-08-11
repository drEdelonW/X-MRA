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

enum LegJoint {
   Coxa,
   Femur,
   Tibia,
   PhalNum
};
class ArachnidLeg {
public:

   ArachnidLeg(
      JointBase*  Jn,
      Millimeters offs = 0.f,
      Angle       rotation = deg(0.f)
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

#if 0
   JointBase& _jnCoxa;
   JointBase& _jnFemur;
   JointBase& _jnTibia;
#else
   JointBase* _jn[PhalNum];
#endif

#if 0
   const Millimeters coxaLength_  = mm( 27.f  ); // TODO: make length configurable by constructor
   const Millimeters femurLength_ = mm( 85.1f );
   const Millimeters tibiaLength_ = mm(144.23f);
#else
   const Millimeters _Length[PhalNum] = {
      [Coxa]  = mm( 27.f  ),
      [Femur] = mm( 85.1f ),
      [Tibia] = mm(144.23f)
   };
#endif
   M4x4 _bodyToLeg, _legToBody;
};
typedef ArachnidLeg* ArachnidLeg_p;
