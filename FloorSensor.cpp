#include "FloorSensor.h"

FloorSensor::FloorSensor(int floorNum, Elevator* elevator){
    this->floorNum = floorNum;
    this->elevator = elevator;
}
void FloorSensor::detected(){
    elevator->updateCurrentFloorNum(floorNum);
    emit signalArrivedAtFloor(elevator->getElevatorNum(), floorNum);
}
