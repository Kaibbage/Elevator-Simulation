#include "Button.h"

Button::Button(){
    illuminated = false;
    turnOffLight();
}

void Button::illuminate(){
    illuminated = true;
    emit isIlluminatedSignal(illuminated);
}

void Button::turnOffLight(){
    illuminated = false;
    emit isIlluminatedSignal(illuminated);
}

bool Button::isIlluminated(){
    return illuminated;
}
