#pragma once
#include "ArachnidLeg.hpp"

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
/* ArachnidBody:
                      ▲ X (forward)
                      │
        FrontLeft     │     FrontRight
           [0]        │        [1]
             \        │        /
              \       │       /
               \      │      /
                \     │     /
                 \    │    /
                  \   │   /
                   \  │  /
MidleLeft           \ │ /            MidleRight
[2] ◀─────────────────┼──────────────────▶ [3]
                    / │ \
                   /  │  \
                  /   │   \
                 /    │    \
                /     │     \
               /      │      \
              /       │       \
           [4]        │        [5]
        BackLeft      │     BackRight
                      │
                      ▼ -X (backward)

               Top-down view of ArachnidBody
             (X — forward, Y — right, Z — up)
*/


class ArachnidBody {
public:
   ArachnidBody();

//    bool  checkJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng);
//    Vector3D   tipPosition(Angle coxaAng, Angle femurAng, Angle tibiaAng);
//    bool checkTipPosition(float x, float y, float z);
//    bool checkTipPosition(Vector3D pos);

//    Vector3D bodyToLeg(Vector3D bodyPos);
//    Vector3D legToBody(Vector3D legPos);

   bool applyPose();

   void activate();
   void deactivate();

private:
//    JointBase& coxaJn_;
//    JointBase& femurJn_;
//    JointBase& tibiaJn_;

};
