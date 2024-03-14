#include "Bell.h"

//constructor
Bell::Bell(int elevatorNum){
    this->elevatorNum = elevatorNum;
}

//Send a signal that the bell has been rung
void Bell::ringBell(){
    emit ringBellSignal(elevatorNum);
}
