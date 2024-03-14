#include "Elevator.h"
#include "ECS.h"

//Constructor
Elevator::Elevator(int elevatorNum, vector<Button*>& destinationButtons, Button* openButton, Button* closeButton, Button* helpButton, Bell* bell, Door* elevatorDoor, Display* display, AudioSystem* audioSystem, WeightSensor* weightSensor, LightSensor* lightSensor, ECS* ecs){
    this->elevatorNum = elevatorNum;

    this->currentWeight = 0;
    this->lastDisplayWasMsg = false;
    this->emergencyMode = false;

    this->destinationButtons = destinationButtons;
    this->openButton = openButton;
    this->closeButton = closeButton;
    this->helpButton = helpButton;
    this->bell = bell;
    this->elevatorDoor = elevatorDoor;
    this->display = display;
    this->audioSystem = audioSystem;
    this->weightSensor = weightSensor;
    this->lightSensor = lightSensor;
    this->ecs = ecs;

    weightOverload = false;
    lightSensorBlockedTwice = false;
    fireTime = false;
    powerOutTime = false;

    outOfOrder = false;

    needHelp = false;
}


//Destructor, deletes all parts of elevator except ecs
Elevator::~Elevator(){
    for(int i = 0; i < NUM_FLOORS; i++){
        delete destinationButtons.at(i);
    }
    delete openButton;
    delete closeButton;
    delete helpButton;
    delete bell;
    delete elevatorDoor;
    delete display;
    delete audioSystem;
    delete weightSensor;
    delete lightSensor;
}

//Tells the elevator to stop at this floor
void Elevator::stopElevator(){
    if(emergencyMode){
        emergencyStop();
    }

    if(destinationButtons.at(getElevatorFloorNum())->isIlluminated()){
        destinationButtons.at(getElevatorFloorNum())->turnOffLight();
    }
    bell->ringBell();
    elevatorDoor->openDoor();


}

//Tells the elevator it is ready to leave
void Elevator::startElevator(){
    bell->ringBell();
    elevatorDoor->closeDoor();
}

//Tells the elevator a destination button has been pressed
void Elevator::pressDestinationButton(int floorNum){
    destinationButtons.at(floorNum)->illuminate();
    ecs->addDestinationFloorRequest(elevatorNum, floorNum);

}

//Tells the elevator to update its floor number
void Elevator::updateCurrentFloorNum(int floorNum){
    display->displayFloor(floorNum);
    ecs->updateElevatorFloor(elevatorNum, floorNum);

}

//getter
int Elevator::getElevatorFloorNum(){
    return ecs->getElevatorFloorNum(elevatorNum);
}

//setter
void Elevator::setECS(ECS* ecs){
    this->ecs = ecs;
}

//increases the elevator's weight by an amount and checks with weight sensor
void Elevator::increaseWeight(int amount){
    if(currentWeight + amount > 0){
        currentWeight += amount;
    }
    else{
        currentWeight = 0;
    }

    if(weightSensor->checkWeightSensorOverload(currentWeight)){
        ecs->weightOverloadRequest(elevatorNum);
        weightOverload = true;
    }

}

//decreases the elevator's weight by an amount and checks with the weight sensor
void Elevator::decreaseWeight(int amount){
    if(currentWeight - amount > 0){
        currentWeight -= amount;
    }
    else{
        currentWeight = 0;
    }

    if(!weightSensor->checkWeightSensorOverload(currentWeight)){
        ecs->weightGoodRequest(elevatorNum);
        weightOverload = false;
    }
}

//stops elevator for weight
void Elevator::stopElevatorForWeight(){
    display->displayMessage("Weight limit exceeded, please lower weight");
    audioSystem->outputAudioMessage("Weight limit exceeded, please lower weight");

}

//blocks elevator light sensor
void Elevator::blockLightSensor(){
    if(!lightSensor->hasLightSensorBeenBlockedBefore()){
        lightSensor->blockLightSensor();
        ecs->blockedDoorOnceRequest(elevatorNum);
    }
    else{
        lightSensorBlockedTwice = true;
        ecs->blockedDoorMultipleRequest(elevatorNum);
    }

}

//tells elevator enough time has passed without the light sensor being blocked
void Elevator::unblockLightSensor(){
    lightSensor->unblockLightSensor();
    lightSensorBlockedTwice = false;
}

//tells elevator the light sensor has been blocked more than once
void Elevator::stopElevatorForBlockedMoreThanOnce(){
    display->displayMessage("Stop blocking the door");
    audioSystem->outputAudioMessage("Stop blocking the door");
}

//Tells the elevator there is a fire
void Elevator::fire(){
    display->displayMessage("There is a fire in this elevator, please exit when the doors open at the next floor");
    audioSystem->outputAudioMessage("There is a fire in this elevator, please exit when the doors open at the next floor");
    emergencyMode = true;
    fireTime = true;
}

//Tells the elevator that it is doing an emergency stop
void Elevator::emergencyStop(){
    display->displayMessage("Please exit immediately");
    audioSystem->outputAudioMessage("Please exit immediately");
    outOfOrder = true;
    ecs->noLongerRunning(elevatorNum);
    fireTime = false;
    powerOutTime = false;
}

//Tells the elevator there is a power outage
void Elevator::powerOutage(){
    display->displayMessage("This elevator is out of power, please exit when the doors open at the next floor");
    audioSystem->outputAudioMessage("This elevator is out of power, please exit when the doors open at the next floor");
    emergencyMode = true;
    powerOutTime = true;
}

//Telling the elevator the help button has been pressed
void Elevator::pressHelpButton(){
    ecs->helpButtonRequest(elevatorNum);
    helpButton->illuminate();
    needHelp = true;
}

//Telling the elevator the user has talked
void Elevator::talk(){
    ecs->talk(elevatorNum);
    needHelp = false;
    helpButton->turnOffLight();
}

//Telling the elevator the open button has been pressed
void Elevator::pressOpenDoorButton(){
    openButton->illuminate();
    ecs->openButtonRequest(elevatorNum);
}

//Telling the elevator the close button has been pressed
void Elevator::pressCloseDoorButton(){
    closeButton->illuminate();
    ecs->closeButtonRequest(elevatorNum);
}

//Telling the elevator the open button has been let go
void Elevator::letGoOpenDoorButton(){
    openButton->turnOffLight();
    ecs->letGoOpenButtonRequest(elevatorNum);
}

//Telling the elevator the close button has been let go
void Elevator::letGoCloseDoorButton(){
    closeButton->turnOffLight();
}

//getters and setters
vector<Button*> Elevator::getDestinationButtons(){
    return destinationButtons;
}
Button* Elevator::getOpenButton(){
    return openButton;
}
Button* Elevator::getCloseButton(){
    return closeButton;
}
Button* Elevator::getHelpButton(){
    return helpButton;
}
Bell* Elevator::getBell(){
    return bell;
}
Door* Elevator::getElevatorDoor(){
    return elevatorDoor;
}
Display* Elevator::getDisplay(){
    return display;
}
AudioSystem* Elevator::getAudioSystem(){
    return audioSystem;
}
WeightSensor* Elevator::getWeightSensor(){
    return weightSensor;
}
LightSensor* Elevator::getLightSensor(){
    return lightSensor;
}
int Elevator::getElevatorNum(){
    return elevatorNum;
}

int Elevator::getCurrentWeight(){
    return currentWeight;
}

bool Elevator::getWeightOverload(){
    return weightOverload;
}
bool Elevator::getLightSensorBlockedTwice(){
    return lightSensorBlockedTwice;
}
bool Elevator::getFireTime(){
    return fireTime;
}
bool Elevator::getPowerOutTime(){
    return powerOutTime;
}


bool Elevator::getOutOfOrder(){
    return outOfOrder;
}

bool Elevator::isDoorOpen(){
    return elevatorDoor->isDoorOpen();
}

bool Elevator::getNeedHelp(){
    return needHelp;
}
void Elevator::setNeedHelp(bool needHelp){
    this->needHelp = needHelp;
    helpButton->turnOffLight();
}
