#include "AudioSystem.h"

void AudioSystem::outputAudioMessage(int elevatorNum, string audioMessage){
    emit outputAudioMessageSignal(elevatorNum, audioMessage);
}
