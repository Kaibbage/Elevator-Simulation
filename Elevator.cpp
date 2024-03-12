#include "Elevator.h"
#include "ECS.h"

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

void Elevator::startElevator(){
    bell->ringBell();
    elevatorDoor->closeDoor();
}

void Elevator::pressDestinationButton(int floorNum){
    destinationButtons.at(floorNum)->illuminate();
    ecs->addDestinationFloorRequest(elevatorNum, floorNum);

}

void Elevator::updateCurrentFloorNum(int floorNum){
    display->displayFloor(elevatorNum, floorNum);
    ecs->updateElevatorFloor(elevatorNum, floorNum);

}

int Elevator::getElevatorFloorNum(){
    return ecs->getElevatorFloorNum(elevatorNum);
}

void Elevator::setECS(ECS* ecs){
    this->ecs = ecs;
}

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
void Elevator::stopElevatorForWeight(){
    display->displayMessage(elevatorNum, "Weight limit exceeded, please lower weight");
    audioSystem->outputAudioMessage(elevatorNum, "Weight limit exceeded, please lower weight");

}

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

void Elevator::unblockLightSensor(){
    lightSensor->unblockLightSensor();
    lightSensorBlockedTwice = false;
}

void Elevator::stopElevatorForBlockedMoreThanOnce(){
    display->displayMessage(elevatorNum, "Stop blocking the door");
    audioSystem->outputAudioMessage(elevatorNum, "Stop blocking the door");
}

void Elevator::fire(){
    display->displayMessage(elevatorNum, "There is a fire in this elevator, please exit when the doors open at the next floor");
    audioSystem->outputAudioMessage(elevatorNum, "There is a fire in this elevator, please exit when the doors open at the next floor");
    emergencyMode = true;
    fireTime = true;
}

void Elevator::emergencyStop(){
    display->displayMessage(elevatorNum, "Please exit immediately");
    audioSystem->outputAudioMessage(elevatorNum, "Please exit immediately");
    outOfOrder = true;
    ecs->noLongerRunning(elevatorNum);
    fireTime = false;
    powerOutTime = false;
}

void Elevator::powerOutage(){
    display->displayMessage(elevatorNum, "This elevator is out of power, please exit when the doors open at the next floor");
    audioSystem->outputAudioMessage(elevatorNum, "This elevator is out of power, please exit when the doors open at the next floor");
    emergencyMode = true;
    powerOutTime = true;
}

void Elevator::pressHelpButton(){
    ecs->helpButtonRequest(elevatorNum);
    helpButton->illuminate();
    needHelp = true;
}
void Elevator::talk(){
    ecs->talk(elevatorNum);
    needHelp = false;
    helpButton->turnOffLight();
}

void Elevator::pressOpenDoorButton(){
    openButton->illuminate();
    ecs->openButtonRequest(elevatorNum);
}
void Elevator::pressCloseDoorButton(){
    closeButton->illuminate();
    ecs->closeButtonRequest(elevatorNum);
}
void Elevator::letGoOpenDoorButton(){
    openButton->turnOffLight();
    ecs->letGoOpenButtonRequest(elevatorNum);
}

void Elevator::letGoCloseDoorButton(){
    closeButton->turnOffLight();
}

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
