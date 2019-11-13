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

        void Start(AAssetManager *mgr);

        void SetGrainsPerSecond(float val);
        void SetGrainDuration(float val);
        void SetGrainSpray(float val);
        void SetGrainFudge(float val);
        void SetGrainIndex(int val);
        void SetGranularMode(int val);

        float GetGrainsPerSecond();
        float GetGrainDuration();
        float GetGrainSpray();
        float GetGrainFudge();
        float GetGrainIndex();

        void ToggleOnOff();
        void Reset();
        void Scramble();
        void Stutter();

        void SetEnvelopeMode(int mode);

        oboe::DataCallbackResult
        onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;


    private:
        AAssetManager *mgr_;
        oboe::AudioStream *stream_;
        Oscillator osc_;
        LinkManager link_manager_;


        std::vector<std::unique_ptr<SoundGenerator>> sound_generators_;

        bool active_ = true;

    private:
        void EmitEvent(TimingData timing_data);


    };


#endif //GRANNYNORMAN_AUDIOENGINE_H
