#include "ArachnidBody.hpp"


void ArachnidBody::setOffs(Vector3D offs, int pattern) {
    _legPattMatrix[pattern].reset();
    _legPattMatrix[pattern] *= Matrix4x4::createTranslation(-offs);
}
bool ArachnidBody::trySetOffs(Vector3D offs, int pattern) {
    setOffs(offs, pattern);
    return
        applyPose(pattern);
}


void ArachnidBody::addOffs(Vector3D offs, int pattern) {
    _legPattMatrix[pattern] *=
        Matrix4x4::createTranslation(-offs);
}
bool ArachnidBody::tryAddOffs(Vector3D offs, int pattern) {
    addOffs(offs, pattern);
    if (applyPose(pattern)) {
        return true;
    }
    addOffs(-offs, pattern);
    return false;
}

void ArachnidBody::ArachnidBody::addRotationOX(Angle angle, int pattern) {
    _legPattMatrix[pattern] *= Matrix4x4::createRotationX(angle.asRadians());
}
bool ArachnidBody::tryAddRotationOX(Angle angle, int pattern) {
    addRotationOX(angle, pattern);
    if (applyPose(pattern)) {
        return true;
    }
    addRotationOX(-angle, pattern);
    return false;
}

void ArachnidBody::ArachnidBody::addRotationOY(Angle angle, int pattern) {
    _legPattMatrix[pattern] *= Matrix4x4::createRotationY(angle.asRadians());
}
bool ArachnidBody::tryAddRotationOY(Angle angle, int pattern) {
    addRotationOY(angle, pattern);
    if (applyPose(pattern)) {
        return true;
    }
    addRotationOY(-angle, pattern);
    return false;
}

void ArachnidBody::ArachnidBody::addRotationOZ(Angle angle, int pattern) {
    _legPattMatrix[pattern] *= Matrix4x4::createRotationZ(angle.asRadians());
}
bool ArachnidBody::tryAddRotationOZ(Angle angle, int pattern) {
    addRotationOZ(angle, pattern);
    if (applyPose(pattern)) {
        return true;
    }
    addRotationOZ(-angle, pattern);
    return false;
}