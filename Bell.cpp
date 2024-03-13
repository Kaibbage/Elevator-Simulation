#include "Bell.h"

Bell::Bell(int elevatorNum){
    this->elevatorNum = elevatorNum;
}

void Bell::ringBell(){
    emit ringBellSignal(elevatorNum);
}
