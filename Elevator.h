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
*/
class Elevator {
public:
    //constructor/destructor
    Elevator(int elevatorNum, vector<Button*>& destinationButtons, Button* openButton, Button* closeButton, Button* helpButton, Bell* bell, Door* elevatorDoor, Display* display, AudioSystem* audioSystem, WeightSensor* weightSensor, LightSensor* lightSensor, ECS* ecs);
    ~Elevator();

    //basic stuff
    void stopElevator();
    void startElevator();
    void pressDestinationButton(int floorNum);
    void updateCurrentFloorNum(int floorNum);
    int getElevatorFloorNum();
    int getElevatorNum();

    void setECS(ECS* ecs);


    //weight stuff
    void increaseWeight(int amount);
    void decreaseWeight(int amount);
    void stopElevatorForWeight();

    //light stuff
    void blockLightSensor();
    void unblockLightSensor();
    void stopElevatorForBlockedMoreThanOnce();

    //fire stuff
    void fire();

    //power outage stuff
    void powerOutage();

    void emergencyStop();

    //help button stuff
    void pressHelpButton();
    void talk();



    //extras
    void pressOpenDoorButton();
    void pressCloseDoorButton();

    void letGoOpenDoorButton();
    void letGoCloseDoorButton();



    //getters
    vector<Button*> getDestinationButtons();
    Button* getOpenButton();
    Button* getCloseButton();
    Button* getHelpButton();
    Bell* getBell();
    Door* getElevatorDoor();
    Display* getDisplay();
    AudioSystem* getAudioSystem();
    WeightSensor* getWeightSensor();
    LightSensor* getLightSensor();
    int getCurrentWeight();

    bool getWeightOverload();
    bool getLightSensorBlockedTwice();
    bool getFireTime();
    bool getPowerOutTime();

    bool getOutOfOrder();

    bool getNeedHelp();
    void setNeedHelp(bool needHelp);

    bool isDoorOpen();


private:
    int elevatorNum;
    int currentWeight;
    bool lastDisplayWasMsg;
    bool emergencyMode;
    vector<Button*> destinationButtons;
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

    bool weightOverload;
    bool lightSensorBlockedTwice;
    bool fireTime;
    bool powerOutTime;

    bool outOfOrder;

    bool needHelp;



};

#endif
