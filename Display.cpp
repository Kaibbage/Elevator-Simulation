#include "Display.h"

Display::Display(int elevatorNum){
    this->elevatorNum = elevatorNum;
}
void Display::displayFloor(int floorNum){
    emit displayFloorSignal(elevatorNum, floorNum);
}

void Display::displayMessage(string message){
    emit displayMessageSignal(elevatorNum, message);
}
