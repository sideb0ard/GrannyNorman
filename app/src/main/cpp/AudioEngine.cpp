//
// Created by Thorsten Sideboard on 2019-10-12.
//

#include <android/log.h>
#include <memory>

#include "AudioEngine.h"
#include "Event.h"
#include "DefJams.h"

#include "oboe/include/oboe/Definitions.h"
#include "oboe/include/oboe/AudioStreamBuilder.h"

#include "SamplePlayer.h"

void AudioEngine::Start(AAssetManager *mgr) {

    mgr_ = mgr;

    oboe::AudioStreamBuilder b;

    b.setFormat(oboe::AudioFormat::Float);
    b.setChannelCount(1);

    b.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    b.setSharingMode(oboe::SharingMode::Exclusive);

    b.setCallback(this);

    b.openStream(&stream_);

    int sample_rate = stream_->getSampleRate();
    link_manager_.SetSampleRate(sample_rate);

    stream_->setBufferSizeInFrames(stream_->getFramesPerBurst() * 2);
    stream_->requestStart();

    auto looper = make_unique<SamplePlayer>(mgr_, "Sounds/pad.wav");
    sound_generators_.emplace_back(std::move(looper));

}

void AudioEngine::EmitEvent() {

    int midi_tick = link_manager_.GetMidiTick();
    Event ev(midi_tick);

    if (midi_tick % PPBAR == 0) {
        ev.is_start_of_bar = true;
    }

    if (midi_tick % 120 == 0) {
        ev.is_thirtysecond = true;

        if (midi_tick % 240 == 0) {
            ev.is_sixteenth = true;

            if (midi_tick % 480 == 0) {
                ev.is_eighth = true;

                if (midi_tick % PPQN == 0)
                    ev.is_quarter = true;
            }
        }
    }


    if (midi_tick % 160 == 0) {
        ev.is_twentyfourth = true;

        if (midi_tick % 320 == 0) {
            ev.is_twelth = true;

            if (midi_tick % 640 == 0) {
                ev.is_sixth = true;

                if (midi_tick % 1280 == 0)
                    ev.is_third = true;
            }
        }
    }


    for (const auto &sg: sound_generators_) {
        sg->EventNotify(ev);
    }
}

oboe::DataCallbackResult
AudioEngine::onAudioReady(oboe::AudioStream *oboeStream, void *audio_data, int32_t num_frames) {

    link_manager_.UpdateFromAudioCallback(num_frames);

    TimingData timing_data = link_manager_.GetTimingData();

    auto data = static_cast<float *>(audio_data);
    for (int i = 0; i < num_frames; i++) {

        double output_val{0};
        for (const auto &sg: sound_generators_) {
            if (sg->IsActive()) {
                StereoValue val = sg->Generate(timing_data);
                // TODO add Stereo Output
                output_val += val.first;
            }
        }

        data[i] = output_val;


        if (link_manager_.IsMidiTick(i)) {
            EmitEvent();

        }
    }


    return
            oboe::DataCallbackResult::Continue;
}

void AudioEngine::SetGrainsPerSecond(float val){

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);

    sound_generators_[0]->SetParam("grains_per_second", val);
}

void AudioEngine::SetGrainDuration(float val){

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);
    sound_generators_[0]->SetParam("grain_duration", val);
}

void AudioEngine::SetGrainSpray(float val){

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);
    sound_generators_[0]->SetParam("grain_spray", val);
}

void AudioEngine::SetGrainFudge(float val){

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);
    sound_generators_[0]->SetParam("grain_fudge", val);
}

void AudioEngine::SetGrainIndex(int val)
{
    sound_generators_[0]->SetParam("grain_index", val);
}

void AudioEngine::SetGranularMode(int val)
{
    sound_generators_[0]->SetParam("granular_mode", val);
}
