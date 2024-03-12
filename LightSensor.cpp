#include "LightSensor.h"


LightSensor::LightSensor(){
    blockedAtLeastOnceBefore = false;
}
bool LightSensor::hasLightSensorBeenBlockedBefore(){
    return blockedAtLeastOnceBefore;
}
void LightSensor::blockLightSensor(){
    blockedAtLeastOnceBefore = true;
}
void LightSensor::unblockLightSensor(){
    blockedAtLeastOnceBefore = false;
}
