#include "ArachnidLeg.hpp"

void ArachnidLeg::configMount(Millimeters offset, Angle yaw) {
    _legToBody.reset(); // begin of forward matrix
    _legToBody *= M4x4::mxRotZ(yaw.asRadians()); {
        _legToBody *= M4x4::mxTrans(Vector3D{ offset, 0.f, 0.f }); {

            _bodyToLeg.reset(); // begin of reverse matrix
        } _bodyToLeg *= M4x4::mxTrans(Vector3D{ -offset, 0.f, 0.f });
    } _bodyToLeg *= M4x4::mxRotZ(-yaw.asRadians());
}


bool ArachnidLeg::_checkTipPosLegSpace(Vector3D pos) {
    float angle[PhalNum];
    {
        angle[Coxa] = atan2(pos.y, pos.x);  //  ahead of leg
    }
    float planarX = len2D(pos.x, pos.y) - _Length[Coxa];
    float planarZ = pos.z;

    float dist = len2D(planarX, planarZ);
    if  (dist < (fabsf(_Length[Femur] - _Length[Tibia])))   // minReach
        return false;   // unreachable
    if  (dist > (_Length[Femur] + _Length[Tibia]))          // maxReach
        return false;   // unreachable

    float a = _Length[Femur];
    float b = _Length[Tibia];
    float c = dist;
    {
        float angleToTarget = atan2f(planarZ, planarX);
        /* correct law-of-cosines */
        float angleFemurRel = acosf(
            ((a*a) + (c*c) - (b*b)) /
            (2.f * a*c)
        );
        angle[Femur] = angleToTarget + angleFemurRel;
    }
    {
        float angleKnee = acosf(
            ((a*a) + (b*b) - (c*c)) /
            (2.f * a*b)
        );
        angle[Tibia] = M_PI - angleKnee;
    }
    return
        checkJointAngles(
            rad( angle[Coxa] ),
            rad(-angle[Femur]),
            rad(-angle[Tibia]) + deg(90.f - 10.9f)
        );
}
bool ArachnidLeg::_checkTipPosLegSpace(Millimeters x, Millimeters y, Millimeters z) {
    return _checkTipPosLegSpace(Vector3D{x, y, z});
}


Vector3D ArachnidLeg::tipPosLegSpace(Angle coxaAng, Angle femurAng, Angle tibiaAng) {
    float aT = (
        tibiaAng - deg(90.f - 10.9f)
    ).asRadians();

    float aF = femurAng.asRadians();
    float knee = aF + aT;               // femur-tibia plane
    float rx =
        _Length[Coxa] +
        _Length[Femur] * cosf(aF) +
        _Length[Tibia] * cosf(knee);    // projection in Coxa plane

    float rz =
        _Length[Femur] * sinf(aF) +
        _Length[Tibia] * sinf(knee);

    float aC = coxaAng.asRadians();
    float cx = cosf(aC);
    float sx = sinf(aC);

    return { (rx * cx), (rx * sx), rz };
}
