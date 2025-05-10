#include "ArachnidLeg.hpp"
#include <cmath>

ArachnidLeg::ArachnidLeg(ProtoServo& coxaServo, ProtoServo& femurServo, ProtoServo& tibiaServo)
    : coxaServo_(coxaServo), femurServo_(femurServo), tibiaServo_(tibiaServo) {}

void ArachnidLeg::setJointAngles(Angle coxaAngle, Angle femurAngle, Angle tibiaAngle) {
    coxaServo_.setAngle(coxaAngle);
    femurServo_.setAngle(femurAngle);
    tibiaServo_.setAngle(tibiaAngle);
}

void ArachnidLeg::deactivate() {
    coxaServo_.disable();
    femurServo_.disable();
    tibiaServo_.disable();
}

void ArachnidLeg::setTipPosition(Millimeters x, Millimeters y, Millimeters z) {
    // Coxa angle from projection onto XY-plane
    float angleCoxa = atan2(y, x);
    coxaServo_.setAngle(rad(angleCoxa));

    // Shift to local femur-tibia plane
    float planarX = sqrt(x * x + y * y) - coxaLength_;
    float planarZ = -z; // downward is positive

    // Distance from femur joint to target
    float dist = sqrt(planarX * planarX + planarZ * planarZ);
    dist = clamp(dist, 1e-3f, femurLength_ + tibiaLength_ - 1e-3f);

    float angleToTarget = atan2(planarZ, planarX);

    float a = femurLength_;
    float b = tibiaLength_;
    float c = dist;

    // Use law of cosines
    float angleA = acos(clamp((b*b + c*c - a*a) / (2*b*c), -1.0f, 1.0f)); // femur
    float angleB = acos(clamp((a*a + b*b - c*c) / (2*a*b), -1.0f, 1.0f)); // tibia

    float angleFemur = angleToTarget + angleA;
    float angleTibia = M_PI - angleB;

    femurServo_.setAngle(rad(angleFemur));
    tibiaServo_.setAngle(rad(angleTibia));
}

// std::tuple<float, float, float> ArachnidLeg::getTipPosition() const {
//     // Stub for forward kinematics to retrieve current tip position
//     // ...
//     return std::make_tuple(0.0f, 0.0f, 0.0f);
// }
