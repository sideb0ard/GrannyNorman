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


namespace grannynorman {

    using namespace oboe;


    class AudioEngine : public AudioStreamCallback {

    public:

        void start(AAssetManager *mgr);

        void tap(bool b);

        virtual DataCallbackResult
        onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames);

        //void LoadSamples();
        //void setFrequency(float d);



    private:
        AAssetManager *mgr_;
        AudioStream *stream_;
        Oscillator osc_;
        LinkManager link_manager_;


        std::vector<std::unique_ptr<grannynorman::SoundGenerator>> sound_generators_;

//    WavData think_sample_;
//    int read_idx_{0};
//    bool active_{false};

        void EmitEvent();

    };

} // namespace

#endif //GRANNYNORMAN_AUDIOENGINE_H
