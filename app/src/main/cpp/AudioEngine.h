//
// Created by Thorsten Sideboard on 2019-10-12.
//

#ifndef GRANNYNORMAN_AUDIOENGINE_H
#define GRANNYNORMAN_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include "oboe/include/oboe/AudioStreamCallback.h"
#include "oboe/include/oboe/Definitions.h"
#include "oboe/samples/shared/Oscillator.h"

using namespace oboe;

class AudioEngine : public AudioStreamCallback{

public:
    void start();
    void tap(bool b);

    virtual DataCallbackResult
    onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames);

    AudioStream *stream;
    Oscillator osc;
};


#endif //GRANNYNORMAN_AUDIOENGINE_H
