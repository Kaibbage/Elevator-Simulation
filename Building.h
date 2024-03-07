#ifndef BUILDING_H
#define BUILDING_H

class ECS;

/*
Building class
Calls ecs for emergencies
ECS can communicate back
contains a pointer to the ecs
accessed as a pointer because it is sometimes accessed by multiple classes and we want them to only access this one building
*/
class Building {
public:
    Building(ECS*);
    void callBuildingSafety();
    void talkToBuildingSafety();
    void call911();

private:
    ECS *ecs;

};

#endif
