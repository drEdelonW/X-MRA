#include "ArachnidBody.hpp"


ArachnidBody::ArachnidBody(ArachnidLeg* legs, size_t legCount):
   _legs(legs),
   _legCount(legCount)
   {

   }

bool ArachnidBody::applyPose() { return false; }

void ArachnidBody::activate() {};
void ArachnidBody::deactivate() {};