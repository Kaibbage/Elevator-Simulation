#ifndef DOOR_H
#define DOOR_H

#include <QObject>

/*
Door class is very simple
Represents an elevator or floor door and just remembers whether it is open or not
Is a QObject
*/
class Door: public QObject {
Q_OBJECT

public:
    Door(int objectNum = -1);
    void openDoor();
    void closeDoor();
    bool isDoorOpen();

private:
    int objectNum;
    bool isOpen;

signals:
    void sendDoorOpenSignal(int objectNum, bool isOpen);
};

#endif
