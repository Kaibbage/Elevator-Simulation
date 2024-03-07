#include "Elevator.h"
#include "Floor.h"
#include "FloorSensor.h"
#include "Building.h"
#include "defs.h"
#include <cmath>
#include <queue>

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
class ECS {
public:
    ECS(vector<Elevator*>& elevators, vector<Floor*>& floors, vector<vector<FloorSensor*>>& allSetsOfFloorSensors, Building* building);
    ~ECS();

    void assignToElevatorQueue(int elevatorNum, int floorNum);
    void addStartingFloorRequest(int floorNum);
    void addDestinationFloorRequest(int elevatorNum, int floorNum);
    void updateElevatorFloor(int elevatorNum, int floorNum);
    void moveElevatorsTowardsDestination();
    int getElevatorFloorNum(int elevatorNum);
    void readyToGoAfterTenSeconds(int elevatorNum);
    void arrivedAtADesiredFloor(int elevatorNum, int floorNum);


    void setStartingFloorValues(vector<int>& startingFloors);

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

    //WeightOverload stuff
    void weightOverloadRequest(int elevatorNum);

    //HelpButton stuff
    void helpButtonRequest(int elevatorNum);
    void talk(int elevatorNum);
    void dontTalkForFiveSeconds(int elevatorNum);

    //Other extras
    void openButtonRequest(int elevatorNum, int floorNum);
    void closeButtonRequest(int elevatorNum, int floorNum);


private:
    vector<Elevator*> elevators;
    vector<Floor*> floors;
    vector<queue<int>> floorQueues;
    vector<vector<FloorSensor*>> allSetsOfFloorSensors;
    vector<int> currentElevatorFloorNumbers;
    vector<Direction> elevatorDirectionValues;
    Building *building;

    vector<int> lastElementsInQueues;
    vector<bool> areElevatorsMoving;


};

#endif
