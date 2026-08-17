#include "ArachnidLeg.hpp"

bool ArachnidLeg::_checkTipPosLegSpace(Vector3D pos) {
    float angle[PhalNum];
    {
        angle[Coxa] = atan2f(pos.y, pos.x);  // ahead of leg
    }
    float planarX = len2D(pos.x, pos.y) - _Phalanx[Coxa].len;
    float planarZ = pos.z;

    float dist = len2D(planarX, planarZ);
    if  (dist < (fabsf(_Phalanx[Femur].len - _Phalanx[Tibia].len)))   // minReach
        return false;   // unreachable
    if  (dist > (_Phalanx[Femur].len + _Phalanx[Tibia].len))          // maxReach
        return false;   // unreachable

    float a = _Phalanx[Femur].len;
    float b = _Phalanx[Tibia].len;
    float c = dist;
    {
        float angleToTarget = -atan2f(planarZ, planarX);
        /* correct law-of-cosines */
        float angleFemurRel = acosf(
            ((a*a) + (c*c) - (b*b)) /
            (2.f * a*c)
        );
        angle[Femur] = angleToTarget - angleFemurRel;
    }
    {
        float angleKnee = acosf(
            ((a*a) + (b*b) - (c*c)) /
            (2.f * a*b)
        );
        angle[Tibia] =  angleKnee - M_PI;
    }
    return
        checkJointAngles(
            (legJn){
                rad( angle[Coxa] ),
                rad( angle[Femur]),
                rad( angle[Tibia]) + deg(90.f - 10.9f)
            }
        );
}

Vector3D ArachnidLeg::tipPosLegSpace(legJn Ang) {
    float aT = (
        Ang[Tibia] - deg(90.f - 10.9f)
    ).asRadians();

    float aF = Ang[Femur].asRadians();
    float knee = aF + aT;               // femur-tibia plane
    float rx =
        _Phalanx[ Coxa].len +
        _Phalanx[Femur].len * cosf(aF) +
        _Phalanx[Tibia].len * cosf(knee);    // projection in Coxa plane

    float rz =
        _Phalanx[Femur].len * sinf(aF) +
        _Phalanx[Tibia].len * sinf(knee);

    float aC = Ang[Coxa].asRadians();
    float cx = cosf(aC);
    float sx = sinf(aC);

    return {
        (rx * cx),
        (rx * sx),
         rz
    };
}
