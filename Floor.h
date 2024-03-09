#include "Button.h"
#include "Door.h"


#ifndef FLOOR_H
#define FLOOR_H

class ECS;

/*
Floor class
Represents a floor
talks to ecs to talk to elevators
gets instructions from ecs
contains a pointer to the ecs
button functionalities are contained in functions, buttons just light up is all
Accessed as a pointer cuz it's a decently big class
*/
class Floor {
public:
    Floor(int floorNum, Button* upButton, Button* downButton, Door* floorDoor, ECS* ecs);
    ~Floor();
    void pressUpButton();
    void pressDownButton();
    void arrived();
    void leaving();

    void setECS(ECS* ecs);

    int getFloorNum();
    Button* getUpButton();
    Button* getDownButton();
    Door* getFloorDoor();


private:
    int floorNum;
    Button *upButton;
    Button *downButton;
    Door *floorDoor;
    ECS *ecs;


};

#endif
