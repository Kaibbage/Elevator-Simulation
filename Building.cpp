#include "Building.h"
#include "ECS.h"

//constructor
Building::Building(ECS* ecs){
    this->ecs = ecs;
}
//send signal that the building has called building safety
void Building::callBuildingSafety(int elevatorNum){
    emit buildingSafetyCalledSignal(elevatorNum);
}
//send signal that the building has seen that the passenger talked
void Building::talkToBuildingSafety(int elevatorNum){
    emit passengerTalkedSignal(elevatorNum);
}
//send signal that the building has called 911
void Building::call911(int elevatorNum){
    emit called911Signal(elevatorNum);

}
//setter
void Building::setECS(ECS* ecs){
    this->ecs = ecs;
}
