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
    ecs->addStartingFloorRequest(floorNum, Direction::Up);
}

void Floor::pressDownButton(){
    downButton->illuminate();
    ecs->addStartingFloorRequest(floorNum, Direction::Down);
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

void Floor::setECS(ECS* ecs){
    this->ecs = ecs;
}


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
