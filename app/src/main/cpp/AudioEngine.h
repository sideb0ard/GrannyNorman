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
        int GetSharingMode();

        void SetGrainsPerSecond(int target, float val);
        void SetGrainDuration(int target, float val);
        void SetGrainSpray(int target, float val);
        void SetGrainFudge(int target, float val);
        void SetGrainIndex(int target, int val);
        void SetGranularMode(int target, int val);
        void SetEnvelopeMode(int target, int mode);

        float GetGrainsPerSecond(int target);
        float GetGrainDuration(int target);
        float GetGrainSpray(int target);
        float GetGrainFudge(int target);
        float GetGrainIndex(int target);
        int GetEnvelopeMode(int target);

        int GetActive(int target);
        int GetLoopMode(int target);

        void ToggleOnOff(int target);
        void Randomize(int target);
        void Reset(int target);
        void Scramble(int target);
        void Stutter(int target);



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
