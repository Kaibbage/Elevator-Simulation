#include "Display.h"

Display::Display(){

}
void Display::displayFloor(int elevatorNum, int floorNum){
    emit displayFloorSignal(elevatorNum, floorNum);
}

void Display::displayMessage(int elevatorNum, string message){
    emit displayMessageSignal(elevatorNum, message);
}
