#include "Building.h"
#include "ECS.h"

Building::Building(ECS* ecs){
    this->ecs = ecs;
}
void Building::callBuildingSafety(int elevatorNum){
    emit buildingSafetyCalledSignal(elevatorNum);
}
void Building::talkToBuildingSafety(int elevatorNum){
    emit passengerTalkedSignal(elevatorNum);
}
void Building::call911(int elevatorNum){
    emit called911Signal(elevatorNum);

}
void Building::setECS(ECS* ecs){
    this->ecs = ecs;
}
