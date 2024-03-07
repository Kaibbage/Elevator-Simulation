#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

/*
LightSensor class is very simple
Holds a variable that tracks whether or not the light sensor has been blocked before
Elevator has one and calls to check status
Not accessed as a pointer because it is small and each floor sensor is only accessed by one object (elevator)
*/
class LightSensor {
public:
    LightSensor();
    bool hasLightSensorBeenBlockedBefore();
    void blockLightSensor();
    void unblockLightSensor();


private:
    bool blockedAtLeastOnceBefore;

};

#endif
