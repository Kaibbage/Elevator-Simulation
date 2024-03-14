#include "Display.h"

//constructor
Display::Display(int elevatorNum){
    this->elevatorNum = elevatorNum;
}

//Send signal to display floor
void Display::displayFloor(int floorNum){
    emit displayFloorSignal(elevatorNum, floorNum);
}

//Send signal to display message
void Display::displayMessage(string message){
    emit displayMessageSignal(elevatorNum, message);
}
