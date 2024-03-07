#include "Door.h"

Door::Door(){
    isOpen = false;
    closeDoor();
}

void Door::openDoor(){
    isOpen = true;
    emit sendDoorOpenSignal(isOpen);
}

void Door::closeDoor(){
    isOpen = false;
    emit sendDoorOpenSignal(isOpen);
}

bool Door::isDoorOpen(){
    return isOpen;
}

