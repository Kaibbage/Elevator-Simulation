#include "Floor.h"
#include "ECS.h"

//Constructor
Floor::Floor(int floorNum, Button* upButton, Button* downButton, Door* floorDoor, ECS* ecs){
    this->floorNum = floorNum;
    this->upButton = upButton;
    this->downButton = downButton;
    this->floorDoor = floorDoor;
    this->ecs = ecs;
}

//Destructor, deletes all parts of floor except ecs
Floor::~Floor(){
    delete upButton;
    delete downButton;
    delete floorDoor;
}

//presses up button, tells ecs and lights button up
void Floor::pressUpButton(){
    upButton->illuminate();
    ecs->addStartingFloorRequest(floorNum, Direction::Up);
}

//presses down button, tells ecs and light button up
void Floor::pressDownButton(){
    downButton->illuminate();
    ecs->addStartingFloorRequest(floorNum, Direction::Down);
}

//Tells floor that an elevator has arrived/opened here
void Floor::arrived(){
    if(downButton->isIlluminated()){
        downButton->turnOffLight();
    }
    if(upButton->isIlluminated()){
        upButton->turnOffLight();
    }

    floorDoor->openDoor();

}

//tells floor that an elevator is leaving
void Floor::leaving(){
    floorDoor->closeDoor();
}

//setter
void Floor::setECS(ECS* ecs){
    this->ecs = ecs;
}

//getters
int Floor::getFloorNum(){
    return floorNum;
}
Button* Floor::getUpButton(){
    return upButton;
}
Button* Floor::getDownButton(){
    return downButton;
}
Door* Floor::getFloorDoor(){
    return floorDoor;
}
