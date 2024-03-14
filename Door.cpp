#include "Door.h"

//constructor
Door::Door(int objectNum){
    this->objectNum = objectNum;
    isOpen = false;
    closeDoor();
}

//Function to open door and send signal
void Door::openDoor(){
    isOpen = true;
    emit sendDoorOpenSignal(objectNum, true);
}

//Function to close door and send signal
void Door::closeDoor(){
    isOpen = false;
    emit sendDoorOpenSignal(objectNum, false);
}

//getter
bool Door::isDoorOpen(){
    return isOpen;
}

