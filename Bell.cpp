#include "Bell.h"

Bell::Bell(string bellMsg){
    bellMessage = bellMsg;
}

void Bell::ringBell(){
    emit ringBellSignal();
}
