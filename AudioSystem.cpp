#include "AudioSystem.h"

AudioSystem::AudioSystem(int elevatorNum){
    this->elevatorNum = elevatorNum;
}

void AudioSystem::outputAudioMessage(string audioMessage){
    emit outputAudioMessageSignal(elevatorNum, audioMessage);
}
