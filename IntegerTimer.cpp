#include "IntegerTimer.h"

//Creates an integer timer, connects the timeout() from a QTimer to the handleTimeout() in this class
IntegerTimer::IntegerTimer(int elevatorNum, QObject *parent) : QTimer(parent) {
    this->elevatorNum = elevatorNum;
    connect(this, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}

//Sets the elevatorNum that this timer is related to
void IntegerTimer::setElevatorNum(int elevatorNum){
    this->elevatorNum = elevatorNum;
}



//Emits a signal for timeout but with an int param
void IntegerTimer::handleTimeout() {
    emit timeoutWithInt(elevatorNum); // Emitting the signal with an integer value
}
