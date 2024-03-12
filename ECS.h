#include "Elevator.h"
#include "Floor.h"
#include "FloorSensor.h"
#include "Building.h"
#include "defs.h"
#include <cmath>
#include <queue>
#include <QObject>
#include "IntegerTimer.h"

#include <vector>

using namespace std;

#ifndef ECS_H
#define ECS_H

/*
ECS class
holds elevator floor queues since this is the one who manages when to stop
big boss
floors use it to talk to elevators
elevators use it to talk to floors
assigns floor requests to elevators
All requests that come to an elevator come here first to get instructions for the elevator
Controls all elevators and all floors
Accessed by classes as a pointer since it is a large object and all classes should access the same ecs
*/
class ECS: public QObject {
    Q_OBJECT
public:
    ECS(vector<Elevator*>& elevators, vector<Floor*>& floors, vector<vector<FloorSensor*>>& allSetsOfFloorSensors, Building* building);
    ~ECS();

    void assignToElevatorQueue(int elevatorNum, int floorNum);
    void addStartingFloorRequest(int floorNum);
    void addDestinationFloorRequest(int elevatorNum, int floorNum);
    void updateElevatorFloor(int elevatorNum, int floorNum);
    void moveElevatorsTowardsDestination();
    int getElevatorFloorNum(int elevatorNum);

    void arrivedAtADesiredFloor(int elevatorNum, int floorNum);


    void setStartingFloorValues(vector<int>& startingFloors);

    vector<Floor*> &getFloors();
    vector<Elevator*> &getElevators();

    //LightSensor stuff
    void blockedDoorOnceRequest(int elevatorNum);
    void blockedDoorMultipleRequest(int elevatorNum);
    void unblockedDoorRequest(int elevatorNum);

    //PowerOutage stuff
    void powerOutageRequest();

    //FireRequest stuff
    void fireRequestFromBuilding();
    void fireRequestFromElevator(int elevatorNum);

    void assignToClosestFloorEmergency(int elevatorNum);

    void noLongerRunning(int elevatorNum);

    //WeightOverload stuff
    void weightOverloadRequest(int elevatorNum);
    void weightGoodRequest(int elevatorNum);

    //HelpButton stuff
    void helpButtonRequest(int elevatorNum);
    void talk(int elevatorNum);

    //Other extras
    void openButtonRequest(int elevatorNum);
    void closeButtonRequest(int elevatorNum);

    void letGoOpenButtonRequest(int elevatorNum);

private:
    vector<Elevator*> elevators;
    vector<Floor*> floors;
    vector<queue<int>> floorQueues;
    vector<vector<FloorSensor*>> allSetsOfFloorSensors;
    vector<int> currentElevatorFloorNumbers;
    vector<Direction> elevatorDirectionValues;
    Building *building;

    vector<int> lastElementsInQueues;
    vector<bool> areElevatorsReadyToMove;

    vector<IntegerTimer*> tenSecondTimers;
    vector<IntegerTimer*> helpButtonTimers;


public slots:
    void readyToGoAfterTenSeconds(int elevatorNum);
    void passengerNoTalk(int elevatorNum);

signals:
    void goBackToFloorDisplaySignal(int elevatorNum);
    void outOfOrderSignal(int elevatorNum);
};

#endif
