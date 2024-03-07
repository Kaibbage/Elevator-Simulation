#include "ECS.h"

ECS::ECS(vector<Elevator*>& elevators, vector<Floor*>& floors, vector<vector<FloorSensor*>>& allSetsOfFloorSensors, Building* building){
    this->elevators = elevators;
    this->floors = floors;
    this->allSetsOfFloorSensors = allSetsOfFloorSensors;
    this->building = building;


    floorQueues = {};
    for(int i = 0; i < elevators.size(); i++){
        floorQueues.push_back({});
    }

    currentElevatorFloorNumbers = {};
    for(int i = 0; i < elevators.size(); i++){
        currentElevatorFloorNumbers.push_back(1);
    }

    elevatorDirectionValues ={};
    for(int i = 0; i < elevators.size(); i++){
        elevatorDirectionValues.push_back(Direction::Neutral);
    }

    lastElementsInQueues ={};
    for(int i = 0; i < elevators.size(); i++){
        lastElementsInQueues.push_back(0);
    }

    areElevatorsMoving ={};
    for(int i = 0; i < elevators.size(); i++){
        areElevatorsMoving.push_back(false);
    }
}
ECS::~ECS(){
    for(int i = 0; i < elevators.size(); i++){
        delete elevators.at(i);
    }
    for(int i = 0; i < floors.size(); i++){
        delete floors.at(i);
    }
    for(int i = 0; i < allSetsOfFloorSensors.size(); i++){
        for(int j = 0; j < allSetsOfFloorSensors.at(i).size(); j++){
            delete allSetsOfFloorSensors.at(i).at(j);
        }
    }
    delete building;
}

void ECS::assignToElevatorQueue(int elevatorNum, int floorNum){
    floorQueues.at(elevatorNum).push(floorNum);
}

void ECS::addStartingFloorRequest(int floorNum){
    int closest = floors.size();

    int chosenElevatorNum = 0;

    bool prio1 = false;
    //bool needDisplay = false;

    for(int i = 0; i < elevators.size(); i++){
        if(floorQueues.at(i).size() == 0 && abs(currentElevatorFloorNumbers.at(i) - floorNum) < closest){
            chosenElevatorNum = i+1;
            closest = abs(currentElevatorFloorNumbers.at(i) - floorNum);
            prio1 = true;
        }
        else if(prio1 == false && abs(lastElementsInQueues.at(i) - floorNum) < closest){
            chosenElevatorNum = i+1;
            closest = abs(lastElementsInQueues.at(i) - floorNum);
        }
    }

    assignToElevatorQueue(chosenElevatorNum, floorNum);
}

void ECS::addDestinationFloorRequest(int elevatorNum, int floorNum){
    assignToElevatorQueue(elevatorNum, floorNum);
}


void ECS::updateElevatorFloor(int elevatorNum, int floorNum){
    currentElevatorFloorNumbers.at(elevatorNum-1) = floorNum;

    if(currentElevatorFloorNumbers.at(elevatorNum-1) == floorQueues.at(elevatorNum-1).front()){
        arrivedAtADesiredFloor(elevatorNum, currentElevatorFloorNumbers.at(elevatorNum-1));
    }
}


void ECS::moveElevatorsTowardsDestination();


int ECS::getElevatorFloorNum(int elevatorNum){
    return currentElevatorFloorNumbers.at(elevatorNum-1);
}


void ECS::readyToGoAfterTenSeconds(int elevatorNum){
    elevators.at(elevatorNum-1)->startElevator();
    floors.at(currentElevatorFloorNumbers.at(elevatorNum-1)-1)->leaving();
}

void ECS::arrivedAtADesiredFloor(int elevatorNum, int floorNum){
    elevators.at(elevatorNum-1)->stopElevator();
    floors.at(floorNum-1)->arrived();

    areElevatorsMoving.at(elevatorNum-1) = false;
}


void ECS::setStartingFloorValues(vector<int>& startingFloors){
    if(startingFloors.size() == currentElevatorFloorNumbers.size()){
        currentElevatorFloorNumbers = startingFloors;
    }

}
