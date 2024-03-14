#include <string>
#include <QObject>

#ifndef DISPLAY_H
#define DISPLAY_H
#include "defs.h"

using namespace std;

/*
Display class is very simple
Represents an elevator display that either shows the floor or an emergency message
Not accessed as a pointer because it is small and each floor sensor is only accessed by one object (elevator)
*/
class Display: public QObject {
    Q_OBJECT

public:
    Display(int = DEFAULT_VALUE);
    void displayMessage(string message);
    void displayFloor(int floorNum);

private:
    int elevatorNum;

signals:
    void displayMessageSignal(int elevatorNum, string message);
    void displayFloorSignal(int elevatorNum, int floorNum);

};

#endif
