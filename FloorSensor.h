#include "Elevator.h"

#ifndef FLOORSENSOR_H
#define FLOORSENSOR_H

/*
FloorSensor class is very simple
Represents a floor sensor that updates an elevator when it gets to a new floor
Each elevator has its own set of floor sensors (there are 3 elevators and 7 floors, so 21 floor sensors)
For the same of the simulation the ECS calls these, this would not be the case in real life, but it makes sense for the simulation
Is a QObject
*/
class FloorSensor: public QObject {
    Q_OBJECT
public:
    FloorSensor(int floorNum, Elevator* elevator);
    void detected();

private:
    int floorNum;
    Elevator *elevator;

signals:
    void signalArrivedAtFloor(int elevatorNum, int floorNum);
};

#endif
