#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include "defs.h"

/*
Button class is very simple
Represents all buttons (destination, up, down, close, open, help)
Just has the functionality of lighting up and turning off the light
The actual functionality of the buttons is in the elevator and floor classes inside functions
Is a QObject
*/
class Button : public QObject{
    Q_OBJECT
public:
    Button(int objectNum = DEFAULT_VALUE, int destinationNum = DEFAULT_VALUE);
    void illuminate();
    void turnOffLight();
    bool isIlluminated(); //BNew

private:
    int destinationNum;
    int objectNum;
    bool illuminated;


signals:
    void isIlluminatedSignal(int objectNum, int destinationNum, bool i);

};

#endif
