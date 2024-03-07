#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>

/*
Button class is very simple
Represents all buttons (destination, up, down, close, open, help)
Just has the functionality of lighting up and turning off the light
The actual functionality of the buttons is in the elevator and floor classes inside functions
Not accessed as a pointer because it is small and each floor sensor is only accessed by one object (elevator or floor)
*/
class Button : public QObject{
    Q_OBJECT
public:
    Button();
    void illuminate();
    void turnOffLight();
    bool isIlluminated(); //BNew

private:
    bool illuminated;

signals:
    void isIlluminatedSignal(bool i);

};

#endif
