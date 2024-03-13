#include "ECS.h"
#include <iostream>

ECS::ECS(vector<Elevator*>& elevators, vector<Floor*>& floors, vector<vector<FloorSensor*>>& allSetsOfFloorSensors, Building* building){
    this->elevators = elevators;
    this->floors = floors;
    this->allSetsOfFloorSensors = allSetsOfFloorSensors;
    this->building = building;


    floorQueues = {};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        floorQueues.push_back({});
    }

    currentElevatorFloorNumbers = {};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        currentElevatorFloorNumbers.push_back(0);
    }

    elevatorDirectionValues ={};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevatorDirectionValues.push_back(Direction::Neutral);
    }

    lastElementsInQueues ={};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        lastElementsInQueues.push_back(0);
    }

    areElevatorsReadyToMove ={};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        areElevatorsReadyToMove.push_back(false);
    }

    tenSecondTimers = {};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        IntegerTimer* tenSecondTimer = new IntegerTimer();
        tenSecondTimer->setElevatorNum(i);
        tenSecondTimers.push_back(tenSecondTimer);
        connect(tenSecondTimers.at(i), SIGNAL(timeoutWithInt(int)), this, SLOT(readyToGoAfterTenSeconds(int)));
    }

    helpButtonTimers = {};
    for(int i = 0; i < NUM_ELEVATORS; i++){
        IntegerTimer* helpButtonTimer = new IntegerTimer();
        helpButtonTimer->setElevatorNum(i);
        helpButtonTimers.push_back(helpButtonTimer);
        connect(helpButtonTimers.at(i), SIGNAL(timeoutWithInt(int)), this, SLOT(passengerNoTalk(int)));
    }



}
ECS::~ECS(){
    for(int i = 0; i < NUM_ELEVATORS; i++){
        delete elevators.at(i);
    }
    for(int i = 0; i < NUM_FLOORS; i++){
        delete floors.at(i);
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        for(int j = 0; j < NUM_FLOORS; j++){
            delete allSetsOfFloorSensors.at(i).at(j);
        }
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        delete tenSecondTimers.at(i);
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        delete helpButtonTimers.at(i);
    }
    delete building;
}

void ECS::assignToElevatorQueue(int elevatorNum, int floorNum){
    floorQueues.at(elevatorNum).push(floorNum);
}

void ECS::addStartingFloorRequest(int floorNum){
    int closest = NUM_FLOORS;

    int chosenElevatorNum = 0;

    bool prio1 = false;

    //perfecto
    for(int i = 0; i < NUM_ELEVATORS; i++){
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
    if(floorQueues.at(elevatorNum).size() == 1 && !elevators.at(elevatorNum)->getElevatorDoor()->isDoorOpen()){
        areElevatorsReadyToMove.at(elevatorNum) = true;
    }
}


void ECS::updateElevatorFloor(int elevatorNum, int floorNum){
    currentElevatorFloorNumbers.at(elevatorNum) = floorNum;

    if(currentElevatorFloorNumbers.at(elevatorNum) == floorQueues.at(elevatorNum).front()){
        arrivedAtADesiredFloor(elevatorNum, currentElevatorFloorNumbers.at(elevatorNum));
    }
}


void ECS::moveElevatorsTowardsDestination(){
    for(int i = 0; i < NUM_ELEVATORS; i++){
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
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME); //only 3 seconds for now
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();
    floorQueues.at(elevatorNum).pop();
    areElevatorsReadyToMove.at(elevatorNum) = false;

    elevators.at(elevatorNum)->stopElevator();
    floors.at(floorNum)->arrived();


}


void ECS::weightOverloadRequest(int elevatorNum){
    elevators.at(elevatorNum)->stopElevatorForWeight();
    tenSecondTimers.at(elevatorNum)->stop();
}

void ECS::weightGoodRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME/5); //waiting for a little bit
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();

    emit goBackToFloorDisplaySignal(elevatorNum);
}


void ECS::blockedDoorOnceRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->stop();
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME); //waiting for a little bit
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();
}

void ECS::blockedDoorMultipleRequest(int elevatorNum){
    blockedDoorOnceRequest(elevatorNum);
    elevators.at(elevatorNum)->stopElevatorForBlockedMoreThanOnce();

}
void ECS::unblockedDoorRequest(int elevatorNum){
    elevators.at(elevatorNum)->unblockLightSensor();
    emit goBackToFloorDisplaySignal(elevatorNum);
}

void ECS::fireRequestFromElevator(int elevatorNum){
    elevators.at(elevatorNum)->fire();
    assignToClosestFloorEmergency(elevatorNum);
    tenSecondTimers.at(elevatorNum)->disconnect();
}

void ECS::assignToClosestFloorEmergency(int elevatorNum){
    while(!floorQueues.at(elevatorNum).empty()){
        floorQueues.at(elevatorNum).pop();
    }
    floorQueues.at(elevatorNum).push(getElevatorFloorNum(elevatorNum));
    areElevatorsReadyToMove.at(elevatorNum) = true;
}

void ECS::noLongerRunning(int elevatorNum){
    emit outOfOrderSignal(elevatorNum);

}

void ECS::fireRequestFromBuilding(){
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevators.at(i)->fire();
        assignToClosestFloorEmergency(i);
        tenSecondTimers.at(i)->disconnect();
    }
}

void ECS::powerOutageRequest(){
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevators.at(i)->powerOutage();
        assignToClosestFloorEmergency(i);
        tenSecondTimers.at(i)->disconnect();
    }
}

void ECS::helpButtonRequest(int elevatorNum){
    helpButtonTimers.at(elevatorNum)->setInterval(TIME_TO_CALL_911);
    helpButtonTimers.at(elevatorNum)->setSingleShot(true);
    helpButtonTimers.at(elevatorNum)->start();
    building->callBuildingSafety(elevatorNum);
}
void ECS::talk(int elevatorNum){
    helpButtonTimers.at(elevatorNum)->stop();
    building->talkToBuildingSafety(elevatorNum);
}

void ECS::passengerNoTalk(int elevatorNum){
    building->call911(elevatorNum);
    elevators.at(elevatorNum)->setNeedHelp(true);
}
void ECS::setStartingFloorValues(vector<int>& startingFloors){
    if(startingFloors.size() == currentElevatorFloorNumbers.size()){
        currentElevatorFloorNumbers = startingFloors;
    }

}

//for pressed
void ECS::openButtonRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->stop();
}
void ECS::closeButtonRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->stop();
    readyToGoAfterTenSeconds(elevatorNum);
}

void ECS::letGoOpenButtonRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME/2); //waiting for half the time
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();
}


vector<Floor*>& ECS::getFloors(){
    return floors;
}

vector<Elevator*>& ECS::getElevators(){
    return elevators;
}
