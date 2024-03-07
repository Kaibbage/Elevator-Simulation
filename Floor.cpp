#include "Floor.h"
#include "ECS.h"

Floor::Floor(int floorNum, Button* upButton, Button* downButton, Door* floorDoor, ECS* ecs){
    this->floorNum = floorNum;
    this->upButton = upButton;
    this->downButton = downButton;
    this->floorDoor = floorDoor;
    this->ecs = ecs;
}

Floor::~Floor(){
    delete upButton;
    delete downButton;
    delete floorDoor;
}

void Floor::pressUpButton(){
    upButton->illuminate();
    ecs->addStartingFloorRequest(floorNum);
}

void Floor::pressDownButton(){
    downButton->illuminate();
    ecs->addStartingFloorRequest(floorNum);
}

void Floor::arrived(){
    if(downButton->isIlluminated()){
        downButton->turnOffLight();
    }
    if(upButton->isIlluminated()){
        upButton->turnOffLight();
    }

    floorDoor->openDoor();

}

void Floor::leaving(){
    floorDoor->closeDoor();
}
