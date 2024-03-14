#include "LightSensor.h"

//constructor
LightSensor::LightSensor(){
    blockedAtLeastOnceBefore = false;
}
//Returns whether light sensor has been blocked at least once
bool LightSensor::hasLightSensorBeenBlockedBefore(){
    return blockedAtLeastOnceBefore;
}
//blocks light sensor
void LightSensor::blockLightSensor(){
    blockedAtLeastOnceBefore = true;
}
//un blocks light sensor
void LightSensor::unblockLightSensor(){
    blockedAtLeastOnceBefore = false;
}
