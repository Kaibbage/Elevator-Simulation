#include <string>
#include <QObject>

#ifndef BELL_H
#define BELL_H

#include "defs.h"

using namespace std;

/*
Bell class is very simple
Represents an elevator Bell that outputs the bell message when called
Not accessed as a pointer because it is small and each floor sensor is only accessed by one object (elevator)
*/

class Bell: public QObject {
    Q_OBJECT

public:
    Bell(int = DEFAULT_VALUE);
    void ringBell();

private:
    int elevatorNum;
signals:
    void ringBellSignal(int);

};

#endif
