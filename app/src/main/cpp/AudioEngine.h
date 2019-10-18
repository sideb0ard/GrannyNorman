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
#include "SoundGenerator.h"



    class AudioEngine : public oboe::AudioStreamCallback {

    public:

        void start(AAssetManager *mgr);

        void tap(bool b);

        oboe::DataCallbackResult
        onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;


    private:
        AAssetManager *mgr_;
        oboe::AudioStream *stream_;
        Oscillator osc_;
        LinkManager link_manager_;


        std::vector<std::unique_ptr<SoundGenerator>> sound_generators_;

    private:
        void EmitEvent();


    };


#endif //GRANNYNORMAN_AUDIOENGINE_H
