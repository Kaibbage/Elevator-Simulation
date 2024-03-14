#include <string>
#include <QObject>

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H
#include "defs.h"

using namespace std;


/*
AudioSystem class is very simple
Represents an elevaotr audio system that outputs whatever message is sent to it
*/
class AudioSystem: public QObject {
    Q_OBJECT

public:
    AudioSystem(int = DEFAULT_VALUE);
    void outputAudioMessage(string audioMessage);

private:
    int elevatorNum;

signals:
    void outputAudioMessageSignal(int elevatorNum, string audioMessage);

};

#endif
