#include "AudioSystem.h"

//constructor
AudioSystem::AudioSystem(int elevatorNum){
    this->elevatorNum = elevatorNum;
}

//Send signal with the audiomessage to be output
void AudioSystem::outputAudioMessage(string audioMessage){
    emit outputAudioMessageSignal(elevatorNum, audioMessage);
}
