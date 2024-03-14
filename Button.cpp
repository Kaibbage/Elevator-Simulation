#include "Button.h"

//Constructor
Button::Button(int objectNum, int destinationNum){
    this->objectNum = objectNum;
    this->destinationNum = destinationNum;
    illuminated = false;
    turnOffLight();
}

//Set illuminated to true, send signal that this button is illuminated
void Button::illuminate(){
    illuminated = true;
    emit isIlluminatedSignal(objectNum, destinationNum, true);
}

//Set illuminated to false, send signal that this button is not illuminated
void Button::turnOffLight(){
    illuminated = false;
    emit isIlluminatedSignal(objectNum, destinationNum, false);
}

//getter
bool Button::isIlluminated(){
    return illuminated;
}
