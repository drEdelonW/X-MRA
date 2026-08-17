#include "ArachnidBody.hpp"
#include "terminal_tools.h"

#define AZIM_LIMIT  (30.f)
#define ELEV_LIMIT  (20.f)

bool ArachnidBody::AimSetAngle(Angle  azimuth, Angle  elevation) {
    if ((fabsf(  azimuth.asDegrees()) > AZIM_LIMIT) ||
        (fabsf(elevation.asDegrees()) > ELEV_LIMIT)
        )   return false;

    _azimuth   = (isnan(  azimuth.asRAW()))?   _azimuth : azimuth;
    _elevation = (isnan(elevation.asRAW()))? _elevation : elevation;
#if 0
    LOG("az[%f],el[%f]\n", _azimuth.asDegrees(), _elevation.asDegrees() );
#endif
    _ctrlMatrix  = M4x4::mxRotY(_elevation.asRadians());
    _ctrlMatrix *= M4x4::mxRotZ(  _azimuth.asRadians());

    return true;
}

Angle ArachnidBody::AimGetAngleAzimuth()    { return _azimuth; }
Angle ArachnidBody::AimGetAngleElevation()  { return _elevation; }
bool ArachnidBody::AimAddAngle(Angle  azimuth, Angle  elevation) {
    return AimSetAngle(
        AimGetAngleAzimuth()   + azimuth,
        AimGetAngleElevation() + elevation
    );
}
void  ArachnidBody::AimGetAngle(Angle_p azimuth, Angle_p elevation) {
    *azimuth    = AimGetAngleAzimuth();
    *elevation  = AimGetAngleElevation();
}
