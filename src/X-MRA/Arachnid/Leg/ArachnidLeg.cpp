#include "ArachnidLeg.hpp"

ArachnidLeg::ArachnidLeg(
    JointBase& coxaJn,
    JointBase& femurJn,
    JointBase& tibiaJn,
    Millimeters offs,
    Angle rotation
):  _jnCoxa(coxaJn),
    _jnFemur(femurJn),
    _jnTibia(tibiaJn) {
    configMount(offs, rotation);
}

bool ArachnidLeg::checkJointAngles(Angle coxaAngle, Angle femurAngle, Angle tibiaAngle) {
    return
        _jnCoxa.checkPose(coxaAngle) &&
        _jnFemur.checkPose(femurAngle) &&
        _jnTibia.checkPose(tibiaAngle);
}

Angle ArachnidLeg::getJointAngles(LegJoint jName){
    switch (jName) {
      case COXA:  return _jnCoxa.getAngle();
      case FEMUR: return _jnFemur.getAngle();
      case TIBIA: return _jnTibia.getAngle();
      default:    return rad(NAN);
   }
}

bool ArachnidLeg::applyPose() {
    return
        _jnTibia.applyPose() &&
        _jnFemur.applyPose() &&
        _jnCoxa.applyPose();
}

void ArachnidLeg::engage() {
    _jnCoxa.engage();
    _jnFemur.engage();
    _jnTibia.engage();
}

void ArachnidLeg::release() {
    _jnCoxa.release();
    _jnFemur.release();
    _jnTibia.release();
}

