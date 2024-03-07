#include "Display.h"

void Display::displayFloor(int floorNum){
    emit displayFloorSignal(floorNum);
}

void Display::displayMessage(string message){
    emit displayMessageSignal(message);
}
