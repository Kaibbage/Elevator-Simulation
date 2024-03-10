#include "Button.h"

Button::Button(int destinationNum){
    this->destinationNum = destinationNum;
    illuminated = false;
    turnOffLight();
}

void Button::illuminate(){
    illuminated = true;
    emit isIlluminatedSignal(destinationNum, true);
}

void Button::turnOffLight(){
    illuminated = false;
    emit isIlluminatedSignal(destinationNum, false);
}

bool Button::isIlluminated(){
    return illuminated;
}
