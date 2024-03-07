#include <vector>

#include "Bell.h"
#include "Door.h"
#include "Display.h"
#include "AudioSystem.h"
#include "WeightSensor.h"
#include "LightSensor.h"
#include "Button.h"

#ifndef ELEVATOR_H
#define ELEVATOR_H

class ECS;

/*
Elevator class
Talks to ECS to get instructions
Passengers talk to elevator, elevator talks to ECS and gets instructions
Checks with light and weight sensors
Contains a poitner to the ecs
accessed as a pointer as it is a huge object and multiple classes may need to access the same elevator (there can be more than 1 tho)
*/
class Elevator {
public:
    Elevator(int elevatorNum, vector<Button*>* destinationButtons, Button* openButton, Button* closeButton, Button* helpButton, Bell* bell, Door* elevatorDoor, Display* display, AudioSystem* audioSystem, WeightSensor* weightSensor, LightSensor* lightSensor, ECS* ecs);
    ~Elevator();

    void stopElevator();
    void startElevator();
    void pressDestinationButton(int floorNum);
    void updateCurrentFloorNum(int floorNum);
    int getElevatorFloorNum();



    //weight stuff
    void increaseWeight(int amount);
    void decreaseWeight(int amount);
    void stopElevatorForWeight();

    //light stuff
    void blockLightSensor();
    void unblockLightSensor();
    void stopElevatorForBlockedDoorOnce();
    void stopElevatorForBlockedMoreThanOnce();

    //fire stuff
    void fire();

    //power outage stuff
    void powerOutage();

    //help button stuff
    void pressHelpButton();
    void talk();
    void dontTalkForFiveSeconds();

    void emergencyStop();

    //extras
    void pressOpenDoorButton();
    void pressCloseDoorButton();


private:
    int elevatorNum;
    int currentWeight;
    bool lastDisplayWasMsg;
    bool emergencyMode;
    vector<Button*>* destinationButtons;
    Button* openButton;
    Button* closeButton;
    Button* helpButton;
    Bell* bell;
    Door* elevatorDoor;
    Display* display;
    AudioSystem* audioSystem;
    WeightSensor* weightSensor;
    LightSensor* lightSensor;
    ECS *ecs;


};

#endif
