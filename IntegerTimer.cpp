#include "IntegerTimer.h"


IntegerTimer::IntegerTimer(int elevatorNum, QObject *parent) : QTimer(parent) {
    this->elevatorNum = elevatorNum;
    connect(this, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}

void IntegerTimer::setElevatorNum(int elevatorNum){
    this->elevatorNum = elevatorNum;
}




void IntegerTimer::handleTimeout() {
    emit timeoutWithInt(elevatorNum); // Emitting the signal with an integer value
}
