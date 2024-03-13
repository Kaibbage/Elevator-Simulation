#include "Button.h"

Button::Button(int objectNum, int destinationNum){
    this->objectNum = objectNum;
    this->destinationNum = destinationNum;
    illuminated = false;
    turnOffLight();
}

void Button::illuminate(){
    illuminated = true;
    emit isIlluminatedSignal(objectNum, destinationNum, true);
}

void Button::turnOffLight(){
    illuminated = false;
    emit isIlluminatedSignal(objectNum, destinationNum, false);
}

bool Button::isIlluminated(){
    return illuminated;
}
