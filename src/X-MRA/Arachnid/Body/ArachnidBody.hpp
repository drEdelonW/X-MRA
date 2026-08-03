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

#if 0
#define LEG_FL   (1 << 0)  // FrontLeft
#define LEG_FR   (1 << 1)  // FrontRight
#define LEG_ML   (1 << 2)  // MidleLeft
#define LEG_MR   (1 << 3)  // MidleRight
#define LEG_BL   (1 << 4)  // BackLeft
#define LEG_BR   (1 << 5)  // BackRight

#define LEGS_FRONT  (LEG_FL | LEG_FR)
#define LEGS_MID    (LEG_ML | LEG_MR)
#define LEGS_BACK   (LEG_BL | LEG_BR)

#define LEGS_LEFT   (LEG_FL | LEG_ML | LEG_BL)
#define LEGS_RIGHT  (LEG_FR | LEG_MR | LEG_BR)

#define LEGS_ALL    (LEG_FL | LEG_FR | LEG_ML | LEG_MR | LEG_BL | LEG_BR)

#define TRIPOD_A  (LEG_FL | LEG_MR | LEG_BL)  // [0, 3, 4]
#define TRIPOD_B  (LEG_FR | LEG_ML | LEG_BR)  // [1, 2, 5]
#else

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
#endif

#define MAX_LEGS (8)
typedef int LegIdx_t;

#define PATTERN_LEG         for (LegIdx_t legIdx = 0; legIdx < _legCount; legIdx++) if (_maskCheck(pattern, legIdx))
#define LEG_ERROR_OK        do { _lastLegError = MAX_LEGS;  return true;    } while (0)
#define LEG_ERROR_DISARMED  do { _lastLegError = MAX_LEGS;  return false;   } while (0)
#define LEG_ERROR           do { _lastLegError = legIdx;    return false;   } while (0)

class ArachnidBody {
public:
    ArachnidBody(ArachnidLeg* legs, size_t legCount);
    ~ArachnidBody();

    // mask (bitmask) see ArachnidBody scheme for accord bits with actual legs
    void    setPatMask(int pattern, LegMask mask);
    LegMask getPatMask(int pattern);

    void setOffs(Vector3D offs, int pattern = 0);
    void addOffs(Vector3D offs, int pattern = 0);
    void addRotationOX(Angle angle, int pattern = 0);
    void addRotationOY(Angle angle, int pattern = 0);
    void addRotationOZ(Angle angle, int pattern = 0);

    bool trySetOffs(Vector3D offs, int pattern = 0);
    bool tryAddOffs(Vector3D offs, int pattern = 0);
    bool tryAddRotationOX(Angle angle, int pattern = 0);
    bool tryAddRotationOY(Angle angle, int pattern = 0);
    bool tryAddRotationOZ(Angle angle, int pattern = 0);

    bool animAngDeg(Vector3D from, Vector3D to, MicroSeconds duration = us(0));


    bool applyPose(int pattern = 0);
    void getMatrix(Matrix4x4_p mx, int pattern = 0);
    void setMatrix(Matrix4x4_p mx, int pattern = 0);

    bool isArmed();
    bool ARM();
    void DISARM();

    bool AimSetAngle(Angle  azimuth, Angle  elevation);
    void AimGetAngle(Angle_p azimuth, Angle_p elevation);
    Angle AimGetAngleAzimuth();
    Angle AimGetAngleElevation();

    bool AimAddAngle(Angle  azimuth, Angle  elevation);

    ArachnidLeg*    _legs = nullptr;
private:
    struct LegExtras {
        Vector3D    defaultPose;
        Vector3D    currentPose;
    };
    inline bool _maskCheck(int pattern, int legIndex) const { return (_legPattMask[pattern] & (1 << legIndex)); }

    size_t  _legCount = 0;

    bool    _isArmed;

    LegMask     _legMaskLimit;
    LegMask     _legPattMask[MAX_LEGS];
    Matrix4x4   _legPattMatrix[MAX_LEGS];
    int8_t      _lastLegError;

    Matrix4x4   _ctrlMatrix;
    Matrix4x4   _stabMatrix;

    LegExtras   _legExtras[MAX_LEGS];

    Angle   _azimuth;
    Angle   _elevation;
};
