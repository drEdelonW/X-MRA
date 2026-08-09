#include "ArachnidBody.hpp"


void ArachnidBody::setOffs(Vector3D offs, pattern_t pattern) {
    _legPattMatrix[pattern].reset();
    _legPattMatrix[pattern] *=
        Matrix4x4::createTranslation(-offs);
}
bool ArachnidBody::trySetOffs(Vector3D offs, pattern_t pattern) {
    setOffs(offs, pattern);
    return
        applyPose(pattern);
}


void ArachnidBody::addOffs(Vector3D offs, pattern_t pattern) {
    _legPattMatrix[pattern] *=
        Matrix4x4::createTranslation(-offs);
}
bool ArachnidBody::tryAddOffs(Vector3D offs, pattern_t pattern) {
    Matrix4x4 popMx = _legPattMatrix[pattern];
    addOffs(offs, pattern);
    if (applyPose(pattern))
        return true;

    _legPattMatrix[pattern] = popMx;
    return false;
}


void ArachnidBody::addRotationOX(Angle angle, pattern_t pattern) {
    _legPattMatrix[pattern] *=
        Matrix4x4::createRotationX(angle.asRadians());
}
bool ArachnidBody::tryAddRotationOX(Angle angle, pattern_t pattern) {
    Matrix4x4 popMx = _legPattMatrix[pattern];
    addRotationOX(angle, pattern);
    if (applyPose(pattern))
        return true;

    _legPattMatrix[pattern] = popMx;
    return false;
}


void ArachnidBody::addRotationOY(Angle angle, pattern_t pattern) {
    _legPattMatrix[pattern] *=
        Matrix4x4::createRotationY(angle.asRadians());
}
bool ArachnidBody::tryAddRotationOY(Angle angle, pattern_t pattern) {
    Matrix4x4 popMx = _legPattMatrix[pattern];
    addRotationOY(angle, pattern);
    if (applyPose(pattern))
        return true;

    _legPattMatrix[pattern] = popMx;
    return false;
}


void ArachnidBody::addRotationOZ(Angle angle, pattern_t pattern) {
    _legPattMatrix[pattern] *=
        Matrix4x4::createRotationZ(angle.asRadians());
}
bool ArachnidBody::tryAddRotationOZ(Angle angle, pattern_t pattern) {
    Matrix4x4 popMx = _legPattMatrix[pattern];
    addRotationOZ(angle, pattern);
    if (applyPose(pattern))
        return true;

    _legPattMatrix[pattern] = popMx;
    return false;
}
