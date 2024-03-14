#include "FloorSensor.h"

//Constructor
FloorSensor::FloorSensor(int floorNum, Elevator* elevator){
    this->floorNum = floorNum;
    this->elevator = elevator;
}
//Updates the elevator on reaching a new floor, sends signal to ui to update display
void FloorSensor::detected(){
    elevator->updateCurrentFloorNum(floorNum);
    emit signalArrivedAtFloor(elevator->getElevatorNum(), floorNum);
}
