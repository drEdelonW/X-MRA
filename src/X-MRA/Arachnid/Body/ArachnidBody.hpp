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
      0---------> Y (right)
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

enum LegBit : uint8_t {
    LEG_FL = (1 << 0), // FrontLeft
    LEG_FR = (1 << 1), // FrontRight
    LEG_ML = (1 << 2), // MidleLeft
    LEG_MR = (1 << 3), // MidleRight
    LEG_BL = (1 << 4), // BackLeft
    LEG_BR = (1 << 5), // BackRight
};

enum LegMask : uint8_t {
    LEGS_FRONT = LEG_FL | LEG_FR,
    LEGS_MID   = LEG_ML | LEG_MR,
    LEGS_BACK  = LEG_BL | LEG_BR,

    LEGS_LEFT  = LEG_FL | LEG_ML | LEG_BL,
    LEGS_RIGHT = LEG_FR | LEG_MR | LEG_BR,

    LEGS_ALL   = LEG_FL | LEG_FR | LEG_ML | LEG_MR | LEG_BL | LEG_BR,

    TRIPOD_A   = LEG_FL | LEG_MR | LEG_BL, // [0, 3, 4]
    TRIPOD_B   = LEG_FR | LEG_ML | LEG_BR, // [1, 2, 5]
};

#define MAX_LEGS (8)
typedef int LegIdx_t;

#define PATTERN_LEG(ptn)    for (LegIdx_t legIdx = 0; legIdx < _legCount; legIdx++) if (_maskCheck(ptn, legIdx))
#define LEG_ERROR_OK        do { _lastLegError = MAX_LEGS;  return true;    } while (0)
#define LEG_ERROR_DISARMED  do { _lastLegError = MAX_LEGS;  return false;   } while (0)
#include "common_tools.h"
#define LEG_ERROR           do { _lastLegError = legIdx;  LOG("[%d]LEG_ERROR\n",legIdx);  return false; } while (0)

typedef unsigned int pattern_t;
class ArachnidBody {
public:
    ArachnidBody(ArachnidLeg_p legs, size_t legCount);
    ~ArachnidBody();

    // mask (bitmask) see ArachnidBody scheme for accord bits with actual legs
    void    setPatMask(pattern_t pattern, LegMask mask);
    LegMask getPatMask(pattern_t pattern);

    void setOffs(Vector3D offs, pattern_t pattern = 0);
    void addOffs(Vector3D offs, pattern_t pattern = 0);
    void addRotationOX(Angle angle, pattern_t pattern = 0);
    void addRotationOY(Angle angle, pattern_t pattern = 0);
    void addRotationOZ(Angle angle, pattern_t pattern = 0);

    bool trySetOffs(Vector3D offs, pattern_t pattern = 0);
    bool tryAddOffs(Vector3D offs, pattern_t pattern = 0);
    bool tryAddRotationOX(Angle angle, pattern_t pattern = 0);
    bool tryAddRotationOY(Angle angle, pattern_t pattern = 0);
    bool tryAddRotationOZ(Angle angle, pattern_t pattern = 0);

    bool animAngDeg(Vector3D from, Vector3D to, MicroSeconds duration = us(0));


    bool applyPose(pattern_t pattern = 0);
    void getMatrix(Matrix4x4_p mx, pattern_t pattern = 0);
    void setMatrix(Matrix4x4_p mx, pattern_t pattern = 0);

    bool isArmed() const { return _isArmed; }
    bool ARM();
    void DISARM();

    bool AimSetAngle(Angle  azimuth, Angle  elevation);
    void AimGetAngle(Angle_p azimuth, Angle_p elevation);
    Angle AimGetAngleAzimuth();
    Angle AimGetAngleElevation();

    bool AimAddAngle(Angle  azimuth, Angle  elevation);

    ArachnidLeg_p   _legs = nullptr;
private:
    struct LegExtras {
        Vector3D    defaultPose;
        Vector3D    currentPose;
    };
    inline bool _maskCheck(pattern_t pattern, int legIndex) const {
        return (_legPattMask[pattern] & (1 << legIndex));
    }

    int _legCount = 0;
    bool _isArmed;

    LegMask _legMaskLimit;
    LegMask _legPattMask[MAX_LEGS];
    M4x4    _legPattMatrix[MAX_LEGS];
    int8_t  _lastLegError;

    M4x4    _ctrlMatrix;
    M4x4    _stabMatrix;

    LegExtras   _legExtras[MAX_LEGS];

    Angle   _azimuth;
    Angle   _elevation;
};
