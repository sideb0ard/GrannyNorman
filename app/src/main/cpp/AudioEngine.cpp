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

    auto drums = make_unique<SamplePlayer>(mgr_, "Sounds/FoolishGPitch.wav");
    sound_generators_.emplace_back(std::move(drums));
    sound_generators_[0]->Random();
//    sound_generators_[0]->SetActive(true);

    auto bass = make_unique<SamplePlayer>(mgr_, "Sounds/front2WestChord1.wav");
    sound_generators_.emplace_back(std::move(bass));
    sound_generators_[1]->Random();
    //sound_generators_[1]->SetActive(true);

    auto strings = make_unique<SamplePlayer>(mgr_, "Sounds/gaborNote.wav");
    sound_generators_.emplace_back(std::move(strings));
    sound_generators_[2]->Random();
//    sound_generators_[2]->SetActive(true);

    auto techno = make_unique<SamplePlayer>(mgr_, "Sounds/808drumloop.wav");
    sound_generators_.emplace_back(std::move(techno));
    sound_generators_[3]->Random();
//    sound_generators_[3]->SetActive(true);


}

void AudioEngine::EmitEvent(TimingData timing_data) {

//    oboe::SharingMode sharing_mode = stream_->getSharingMode();
//    if (sharing_mode != oboe::SharingMode::Exclusive) {
//        __android_log_print(ANDROID_LOG_ERROR, "WOOF",
//                            "SHARING MODE IS NOTE EXCLUSIVE!!");
//    }

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

    ev.timing_data = timing_data;


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

        if (active_) {
            for (const auto &sg: sound_generators_) {
                if (sg->IsActive()) {
                    StereoValue val = sg->Generate(timing_data);
                    // TODO add Stereo Output
                    output_val += val.first;
                }
            }
        }

        data[i] = output_val;


        if (link_manager_.IsMidiTick(i)) {
            EmitEvent(timing_data);

        }
    }


    return
            oboe::DataCallbackResult::Continue;
}

void AudioEngine::SetGrainsPerSecond(int target, float val) {

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);

    sound_generators_[target]->SetParam("grains_per_second", val);
}

void AudioEngine::SetGrainDuration(int target, float val) {

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);
    sound_generators_[target]->SetParam("grain_duration", val);
}

void AudioEngine::SetGrainSpray(int target, float val) {

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);
    sound_generators_[target]->SetParam("grain_spray", val);
}

void AudioEngine::SetGrainFudge(int target, float val) {

    __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                        "VVAL:%F", val);
    sound_generators_[target]->SetParam("grain_fudge", val);
}

void AudioEngine::SetGrainIndex(int target, int val) {
    sound_generators_[target]->SetParam("grain_index", val);
}

void AudioEngine::SetGranularMode(int target, int val) {
    sound_generators_[target]->SetParam("granular_mode", val);
}

void AudioEngine::SetEnvelopeMode(int target, int val) {
    sound_generators_[target]->SetParam("envelope_mode", val);
}

///

float AudioEngine::GetGrainsPerSecond(int target) {

    return sound_generators_[target]->GetParam("grains_per_second");
}

float AudioEngine::GetGrainDuration(int target) {

    return sound_generators_[target]->GetParam("grain_duration");
}

float AudioEngine::GetGrainSpray(int target) {


    return sound_generators_[target]->GetParam("grain_spray");
}

float AudioEngine::GetGrainFudge(int target) {


    return sound_generators_[target]->GetParam("grain_fudge");
}

float AudioEngine::GetGrainIndex(int target) {
    return sound_generators_[target]->GetParam("grain_index");
}

int AudioEngine::GetEnvelopeMode(int target) {
    return sound_generators_[target]->GetParam("envelope_mode");
}

void AudioEngine::ToggleOnOff(int target) {
    bool active = 1 - sound_generators_[target]->IsActive();
    return sound_generators_[target]->SetActive(active);
}


void AudioEngine::Randomize(int target) {
    return sound_generators_[target]->Random();
}

void AudioEngine::Reset(int target) {
    return sound_generators_[target]->Reset();
}

void AudioEngine::Scramble(int target) {
    return sound_generators_[target]->SetParam("scramble_mode", 1);
}

void AudioEngine::Stutter(int target) {
    return sound_generators_[target]->SetParam("stutter_mode", 1);
}


int AudioEngine::GetActive(int target) {
    return sound_generators_[target]->IsActive();
}

int AudioEngine::GetLoopMode(int target) {
    return sound_generators_[target]->GetParam("loop_mode");
}

int AudioEngine::GetSharingMode() {
    oboe::SharingMode sharing_mode = stream_->getSharingMode();
    if (sharing_mode == oboe::SharingMode::Exclusive)
        return 0;
    return 1;
}


