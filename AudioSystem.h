#include <string>
#include <QObject>

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

using namespace std;


/*
AudioSystem class is very simple
Represents an elevaotr audio system that outputs whatever message is sent to it
*/
class AudioSystem: public QObject {
    Q_OBJECT

public:
    void outputAudioMessage(int elevatorNum, string audioMessage);

private:

signals:
    void outputAudioMessageSignal(int elevatorNum, string audioMessage);

};

#endif
