#pragma once
#include "ArachnidLeg.hpp"

/*
       ^ Z (up)
       |
       |    7 X (forward)
       |   /
       |  /
       | /
       |/
       *---------> Y (right)
*/
/* ArachnidBody:
                      ^ X (forward)
                      |
        FrontLeft     |     FrontRight
           [0]        |        [1]
             \        |        /
              \       |       /
               \      |      /
                \     |     /
                 \    |    /
                  \   |   /
                   \  |  /
MidleLeft           \ | /            MidleRight
[2] <-----------------+------------------> [3]
                    / | \
                   /  |  \
                  /   |   \
                 /    |    \
                /     |     \
               /      |      \
              /       |       \
           [4]        |        [5]
        BackLeft      |     BackRight
                      |
                      v -X (backward)

               Top-down view of ArachnidBody
             (X - forward, Y - right, Z - up)
*/

#define MAX_LEGS  (8)

class ArachnidBody {
public:
    ArachnidBody(ArachnidLeg* legs, size_t legCount);

    bool applyPose();

    bool ARM();
    void DISARM();

    ArachnidLeg*    _legs = nullptr;
private:
    size_t          _legCount = 0;
    Vector3D        _defaultPose[MAX_LEGS];
    Vector3D        _curentPose[MAX_LEGS];

    uint8_t         _legMask[MAX_LEGS];
    uint8_t         _legMaskLimit;
    bool            _isArmed;
};
