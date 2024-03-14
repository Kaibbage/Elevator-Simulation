#ifndef BUILDING_H
#define BUILDING_H
#include <QObject>

class ECS;

/*
Building class
Calls ecs for emergencies
ECS can communicate back
contains a pointer to the ecs
accessed as a pointer because it is sometimes accessed by multiple classes and we want them to only access this one building
Is a QObject
*/
class Building: public QObject {
    Q_OBJECT
public:
    Building(ECS*);
    void callBuildingSafety(int elevatorNum);
    void talkToBuildingSafety(int elevatorNum);
    void call911(int elevatorNum);
    void setECS(ECS*);

private:
    ECS *ecs;
signals:
    void buildingSafetyCalledSignal(int elevatorNum);
    void passengerTalkedSignal(int elevatorNum);
    void called911Signal(int elevatorNum);

};

#endif
