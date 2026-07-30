#include "ArachnidBody.hpp"
#include "terminal_tools.h"

#define AZIM_LIMIT      (30)
#define ELEV_LIMIT      (20)

bool ArachnidBody::AimSetAngle(Angle  azimuth, Angle  elevation) {
    if (isnan(azimuth.asRadians()) ||
        isnan(elevation.asRadians())
        ) {
        if (!isnan(azimuth.asRadians())) {
            if (fabsf(azimuth.asDegrees()) > AZIM_LIMIT)
                return false;

            _azimuth = azimuth;
        }
        else {
            if (fabsf(elevation.asDegrees()) > ELEV_LIMIT)
                return false;

            _elevation = elevation;
        }
    }
    else {
        if ((fabsf(azimuth.asDegrees()) > AZIM_LIMIT) ||
            (fabsf(elevation.asDegrees()) > ELEV_LIMIT)
            )   return false;

        _azimuth = azimuth;
        _elevation = elevation;
    }
    LOG("az[%f],el[%f]\n",
        _azimuth.asDegrees(),
        _elevation.asDegrees()
    );
    _ctrlMatrix = Matrix4x4::createRotationY(_elevation.asRadians());
    _ctrlMatrix *= Matrix4x4::createRotationZ(_azimuth.asRadians());

    return true;
}
bool  ArachnidBody::AimAddAngle(Angle  azimuth, Angle  elevation) {
    return AimSetAngle(
        _azimuth + azimuth,
        _elevation + elevation
    );
}

void ArachnidBody::AimGetAngle(Angle_p azimuth, Angle_p elevation) {
    *azimuth = _azimuth;
    *elevation = _elevation;
}
Angle ArachnidBody::AimGetAngleAzimuth() {
    return _azimuth;
}
Angle ArachnidBody::AimGetAngleElevation() {
    return _elevation;
}

