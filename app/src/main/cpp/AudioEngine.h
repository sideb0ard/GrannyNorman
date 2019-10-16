//
// Created by Thorsten Sideboard on 2019-10-12.
//

#ifndef GRANNYNORMAN_AUDIOENGINE_H
#define GRANNYNORMAN_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include <android/asset_manager.h>
#include "oboe/include/oboe/AudioStreamCallback.h"
#include "oboe/include/oboe/Definitions.h"
#include "oboe/samples/shared/Oscillator.h"

#include "WavData.h"
#include "LinkManager.h"

using namespace oboe;

class AudioEngine : public AudioStreamCallback{

public:

    void start();
    void tap(bool b);

    virtual DataCallbackResult
    onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames);

    void LoadSamples(AAssetManager *mgr);
    void setFrequency(float d);



private:
    AudioStream *stream_;
    Oscillator osc_;
    LinkManager link_manager_;

    WavData think_sample_;
    int read_idx_{0};

    bool active_{false};

    void EmitEvent();

};


#endif //GRANNYNORMAN_AUDIOENGINE_H
