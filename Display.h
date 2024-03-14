#include <string>
#include <QObject>

#ifndef DISPLAY_H
#define DISPLAY_H
#include "defs.h"

using namespace std;

/*
Display class is very simple
Represents an elevator display that either shows the floor or an emergency message
Is a QObject
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
