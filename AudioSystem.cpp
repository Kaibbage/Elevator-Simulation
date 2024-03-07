#include "AudioSystem.h"

void AudioSystem::outputAudioMessage(string audioMessage){
    emit outputAudioMessageSignal(audioMessage);
}
