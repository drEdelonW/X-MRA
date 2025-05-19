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
   ArachnidBody(ArachnidLeg* legs, size_t legCount);

   bool applyPose();

   void activate();
   void deactivate();

    ArachnidLeg* _legs = nullptr;
    size_t       _legCount = 0;
private:

};
