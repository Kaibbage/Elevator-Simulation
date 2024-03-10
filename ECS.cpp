#include "ECS.h"
#include <iostream>

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
        currentElevatorFloorNumbers.push_back(0);
    }

    elevatorDirectionValues ={};
    for(int i = 0; i < elevators.size(); i++){
        elevatorDirectionValues.push_back(Direction::Neutral);
    }

    lastElementsInQueues ={};
    for(int i = 0; i < elevators.size(); i++){
        lastElementsInQueues.push_back(0);
    }

    areElevatorsReadyToMove ={};
    for(int i = 0; i < elevators.size(); i++){
        areElevatorsReadyToMove.push_back(false);
    }

    connect(&tenSecondTimer, SIGNAL(timeoutWithInt(int)), this, SLOT(readyToGoAfterTenSeconds(int)));
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
    cout << "hihi" << endl;
    floorQueues.at(elevatorNum).push(floorNum);
}

void ECS::addStartingFloorRequest(int floorNum){
    int closest = floors.size();

    int chosenElevatorNum = 0;

    bool prio1 = false;
    //bool needDisplay = false;

    //This logic isn't perfect, need to fix
    for(int i = 0; i < elevators.size(); i++){
        if(floorQueues.at(i).size() == 0 && (abs(currentElevatorFloorNumbers.at(i) - floorNum) < closest || prio1 == false)){
            chosenElevatorNum = i;
            closest = abs(currentElevatorFloorNumbers.at(i) - floorNum);
            prio1 = true;
        }
        else if(prio1 == false && abs(lastElementsInQueues.at(i) - floorNum) < closest){
            chosenElevatorNum = i;
            closest = abs(lastElementsInQueues.at(i) - floorNum);
        }
    }

    assignToElevatorQueue(chosenElevatorNum, floorNum);
    if(floorQueues.at(chosenElevatorNum).size() == 1){
        areElevatorsReadyToMove.at(chosenElevatorNum) = true;
    }

}

void ECS::addDestinationFloorRequest(int elevatorNum, int floorNum){
    assignToElevatorQueue(elevatorNum, floorNum);
    if(floorQueues.at(elevatorNum).size() == 1){
        areElevatorsReadyToMove.at(elevatorNum) = true;
    }
}


void ECS::updateElevatorFloor(int elevatorNum, int floorNum){
    currentElevatorFloorNumbers.at(elevatorNum) = floorNum;
    cout << elevatorNum << " " << currentElevatorFloorNumbers.at(elevatorNum) << " " << floorQueues.at(elevatorNum).front() << endl;

    if(currentElevatorFloorNumbers.at(elevatorNum) == floorQueues.at(elevatorNum).front()){
        arrivedAtADesiredFloor(elevatorNum, currentElevatorFloorNumbers.at(elevatorNum));
    }
}


void ECS::moveElevatorsTowardsDestination(){
    for(int i = 0; i < elevators.size(); i++){
        if(areElevatorsReadyToMove.at(i) && floorQueues.at(i).size() > 0){
            if(currentElevatorFloorNumbers.at(i) < floorQueues.at(i).front()){
                allSetsOfFloorSensors.at(i).at(currentElevatorFloorNumbers.at(i)+1)->detected();
            }
            else if(currentElevatorFloorNumbers.at(i) > floorQueues.at(i).front()){
                allSetsOfFloorSensors.at(i).at(currentElevatorFloorNumbers.at(i)-1)->detected();
            }
            else{
                allSetsOfFloorSensors.at(i).at(currentElevatorFloorNumbers.at(i))->detected();
            }
        }


    }
}


int ECS::getElevatorFloorNum(int elevatorNum){
    return currentElevatorFloorNumbers.at(elevatorNum);
}


void ECS::readyToGoAfterTenSeconds(int elevatorNum){
    elevators.at(elevatorNum)->startElevator();
    floors.at(currentElevatorFloorNumbers.at(elevatorNum))->leaving();

    areElevatorsReadyToMove.at(elevatorNum) = true;
}

void ECS::arrivedAtADesiredFloor(int elevatorNum, int floorNum){
    tenSecondTimer.setElevatorNum(elevatorNum);
    tenSecondTimer.setInterval(3000); //only 3 seconds for now
    tenSecondTimer.setSingleShot(true);
    tenSecondTimer.start();

    floorQueues.at(elevatorNum).pop();
    areElevatorsReadyToMove.at(elevatorNum) = false;

    elevators.at(elevatorNum)->stopElevator();
    floors.at(floorNum)->arrived();


}


void ECS::setStartingFloorValues(vector<int>& startingFloors){
    if(startingFloors.size() == currentElevatorFloorNumbers.size()){
        currentElevatorFloorNumbers = startingFloors;
    }

}


vector<Floor*>& ECS::getFloors(){
    return floors;
}

vector<Elevator*>& ECS::getElevators(){
    return elevators;
}
