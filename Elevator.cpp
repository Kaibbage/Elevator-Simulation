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
}

Elevator::~Elevator(){
    for(int i = 0; i < destinationButtons.size(); i++){
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
    display->displayFloor(floorNum);
    ecs->updateElevatorFloor(elevatorNum, floorNum);

}

int Elevator::getElevatorFloorNum(){
    return ecs->getElevatorFloorNum(elevatorNum);
}

void Elevator::setECS(ECS* ecs){
    this->ecs = ecs;
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
