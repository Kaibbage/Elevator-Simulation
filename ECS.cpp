#include "ECS.h"
#include <iostream>

//Constructor, sets some attributes to default values, sets others to parameters
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

//Destructor, deletes all elevators and floors as well as other things belonging to ECS
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

//Assigns a floor to an elevator
void ECS::assignToElevatorQueue(int elevatorNum, int floorNum){
    floorQueues.at(elevatorNum).push(floorNum);
    lastElementsInQueues.at(elevatorNum) = floorNum;
}

//Assigns a floor request to an elevator
void ECS::addStartingFloorRequest(int floorNum, Direction direction){
    int closest = NUM_FLOORS;

    int chosenElevatorNum = 0;

    bool prio1 = false;

    //perfecto
    for(int i = 0; i < NUM_ELEVATORS; i++){
        //scenario where at least 1 elevator is empty
        if(floorQueues.at(i).size() == 0 && (abs(currentElevatorFloorNumbers.at(i) - floorNum) < closest || prio1 == false)){
            chosenElevatorNum = i;
            closest = abs(currentElevatorFloorNumbers.at(i) - floorNum);
            prio1 = true;
        }
        //scenario where all elevators are full
        else if(prio1 == false && abs(lastElementsInQueues.at(i) - floorNum) < closest){
            chosenElevatorNum = i;
            closest = abs(lastElementsInQueues.at(i) - floorNum);
        }
    }

    assignToElevatorQueue(chosenElevatorNum, floorNum);
    elevatorDirectionValues.at(chosenElevatorNum) = direction;
    if(floorQueues.at(chosenElevatorNum).size() == 1 && !elevators.at(chosenElevatorNum)->getOutOfOrder()){
        areElevatorsReadyToMove.at(chosenElevatorNum) = true;
    }

}

//Assigns a destination request to an elevator
void ECS::addDestinationFloorRequest(int elevatorNum, int floorNum){
    assignToElevatorQueue(elevatorNum, floorNum);
    if(floorQueues.at(elevatorNum).size() == 1 && !elevators.at(elevatorNum)->getElevatorDoor()->isDoorOpen()){
        areElevatorsReadyToMove.at(elevatorNum) = true;
    }

    if(getElevatorFloorNum(elevatorNum) < floorNum){
        elevatorDirectionValues.at(elevatorNum) = Direction::Up;
    }
    else if(getElevatorFloorNum(elevatorNum) > floorNum){
        elevatorDirectionValues.at(elevatorNum) = Direction::Down;
    }
}

//Updates the floor for an elevator, if it is at the front of the queue then arriving
void ECS::updateElevatorFloor(int elevatorNum, int floorNum){
    currentElevatorFloorNumbers.at(elevatorNum) = floorNum;

    if(currentElevatorFloorNumbers.at(elevatorNum) == floorQueues.at(elevatorNum).front()){
        arrivedAtADesiredFloor(elevatorNum, currentElevatorFloorNumbers.at(elevatorNum));
    }
}

//If elevators can move, they are moved one floor towards their destination
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

//getter
int ECS::getElevatorFloorNum(int elevatorNum){
    return currentElevatorFloorNumbers.at(elevatorNum);
}

//Function called 10 seconds after arriving/opening doors to leave/close doors
void ECS::readyToGoAfterTenSeconds(int elevatorNum){
    elevators.at(elevatorNum)->startElevator();
    floors.at(currentElevatorFloorNumbers.at(elevatorNum))->leaving();

    areElevatorsReadyToMove.at(elevatorNum) = true;
}


//Function to start timer and tell elevators and floors that they've arrived
void ECS::arrivedAtADesiredFloor(int elevatorNum, int floorNum){
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME); //only 3 seconds for now
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();
    floorQueues.at(elevatorNum).pop();
    areElevatorsReadyToMove.at(elevatorNum) = false;

    elevators.at(elevatorNum)->stopElevator();
    floors.at(floorNum)->arrived();


}

//Request from elevator that has a weight overload, stopping timer
void ECS::weightOverloadRequest(int elevatorNum){
    elevators.at(elevatorNum)->stopElevatorForWeight();
    tenSecondTimers.at(elevatorNum)->stop();
}
'
//Request from elevator that has escaped a wait overload, restarting timer as 2 seconds
void ECS::weightGoodRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME/5); //waiting for a little bit
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();

    emit goBackToFloorDisplaySignal(elevatorNum);
}

//Request from elevator that has its doors blocked once, restart timer
void ECS::blockedDoorOnceRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->stop();
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME); //waiting for a little bit
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();
}

//Request from elevator that has its doors blocked more than once, restart timer and output messages
void ECS::blockedDoorMultipleRequest(int elevatorNum){
    blockedDoorOnceRequest(elevatorNum);
    elevators.at(elevatorNum)->stopElevatorForBlockedMoreThanOnce();

}

//Request from elevator that the doors are no longer blocked when the timer ends
void ECS::unblockedDoorRequest(int elevatorNum){
    elevators.at(elevatorNum)->unblockLightSensor();
    emit goBackToFloorDisplaySignal(elevatorNum);
}

//Request from elevator that there is a fire, assigns to closest floor
void ECS::fireRequestFromElevator(int elevatorNum){
    elevators.at(elevatorNum)->fire();
    assignToClosestFloorEmergency(elevatorNum);
    tenSecondTimers.at(elevatorNum)->disconnect();
}

//Empties current e queue and assigns an elevator to the closest floor in case of emergency
void ECS::assignToClosestFloorEmergency(int elevatorNum){
    while(!floorQueues.at(elevatorNum).empty()){
        floorQueues.at(elevatorNum).pop();
    }
    assignToElevatorQueue(elevatorNum, getElevatorFloorNum(elevatorNum));
    areElevatorsReadyToMove.at(elevatorNum) = true;
}

//Sends a signal that an elevator is out of order after an emergency
void ECS::noLongerRunning(int elevatorNum){
    emit outOfOrderSignal(elevatorNum);

}

//Request from building for a fire, tells all elevators there's a fire
void ECS::fireRequestFromBuilding(){
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevators.at(i)->fire();
        assignToClosestFloorEmergency(i);
        tenSecondTimers.at(i)->disconnect();
    }
}

//Request from building for a power outage, tells all elevators there's a power outage
void ECS::powerOutageRequest(){
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevators.at(i)->powerOutage();
        assignToClosestFloorEmergency(i);
        tenSecondTimers.at(i)->disconnect();
    }
}

//Request from elevator that the help button has been pressed, starts a timer to call 911
void ECS::helpButtonRequest(int elevatorNum){
    helpButtonTimers.at(elevatorNum)->setInterval(TIME_TO_CALL_911);
    helpButtonTimers.at(elevatorNum)->setSingleShot(true);
    helpButtonTimers.at(elevatorNum)->start();
    building->callBuildingSafety(elevatorNum);
}

//The user talks, elevator tells ECS with this function, stops timer
void ECS::talk(int elevatorNum){
    helpButtonTimers.at(elevatorNum)->stop();
    building->talkToBuildingSafety(elevatorNum);
}

//The user doesn't talk before the timer ends and 911 is called thru the building
void ECS::passengerNoTalk(int elevatorNum){
    building->call911(elevatorNum);
    elevators.at(elevatorNum)->setNeedHelp(true);
}

//Setter for starting floor values
void ECS::setStartingFloorValues(vector<int>& startingFloors){
    if(startingFloors.size() == currentElevatorFloorNumbers.size()){
        currentElevatorFloorNumbers = startingFloors;
    }

}

//for open button pressed, stops timer to leave
void ECS::openButtonRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->stop();
}

//For close button pressed, stops timer to leave and leaves immediately
void ECS::closeButtonRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->stop();
    readyToGoAfterTenSeconds(elevatorNum);
}

//For open button let go, timer to leave restarted at 5 seconds
void ECS::letGoOpenButtonRequest(int elevatorNum){
    tenSecondTimers.at(elevatorNum)->setInterval(ELEVATOR_WAIT_TIME/2); //waiting for half the time
    tenSecondTimers.at(elevatorNum)->setSingleShot(true);
    tenSecondTimers.at(elevatorNum)->start();
}

//getters
vector<Floor*>& ECS::getFloors(){
    return floors;
}

vector<Elevator*>& ECS::getElevators(){
    return elevators;
}
