#pragma once
#include "Matrix4x4.hpp" // #include "Vector3d.hpp"
#include "Phalanx.hpp"

enum LegJoint {
    Coxa,
    Femur,
    Tibia,
    PhalNum
};

/*  ArachnidLeg:  [Tibia]
                    /|
                   / |
   [Body]         /  |
      |          /   |
    [Coxa]--[Femur]  |
      |              |
   [Body]            |
                     |
==============={Ground}==============
*/
typedef Phalanx_t sLeg_t[PhalNum];

typedef Angle legJn[PhalNum];
#define JFREEZE  rad(NAN)

class ArachnidLeg {
public:
    ArachnidLeg(
        Phalanx_p   Phalanx,
        Millimeters offs = 0.f,
        Angle       rotation = deg(0.f)
    );

    void engage();
    void release();
    bool applyPose();

  /*-------------------[Angle]--------------------*/
    Angle   getJointAngles(LegJoint jName);
    bool  checkJointAngles(legJn Ang);
    bool    tryJointAngles(legJn Ang);

  /*----------------[Angle to Pos]----------------*/
    Vector3D   tipPosLegSpace (legJn Ang);
    Vector3D   tipPosBodySpace(legJn Ang);
  /*---------------------[Pos]--------------------*/
    bool  checkTipPosLegSpace (Vector3D legPos);
    bool  checkTipPosBodySpace(Vector3D bodyPos);

private:
    Phalanx_t _Phalanx[PhalNum];

    bool _checkTipPosLegSpace(Vector3D pos);

    M4x4 _bodyToLeg;
    Vector3D bodyToLeg(Vector3D bodyPos);

    M4x4 _legToBody;
    Vector3D legToBody(Vector3D legPos);
};
typedef ArachnidLeg* ArachnidLeg_p;
