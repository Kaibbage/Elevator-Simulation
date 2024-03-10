#include "Door.h"

Door::Door(int objectNum){
    this->objectNum = objectNum;
    isOpen = false;
    closeDoor();
}

void Door::openDoor(){
    isOpen = true;
    emit sendDoorOpenSignal(objectNum, true);
}

void Door::closeDoor(){
    isOpen = false;
    emit sendDoorOpenSignal(objectNum, false);
}

bool Door::isDoorOpen(){
    return isOpen;
}

