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
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/strstream"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdint"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/aaudio/AAudio.h"
#include "SamplePlayer.h"

namespace grannynorman {

    void AudioEngine::start(AAssetManager *mgr) {

        mgr_ = mgr;

        AudioStreamBuilder b;

        b.setFormat(AudioFormat::Float);
        b.setChannelCount(1);

        b.setPerformanceMode(PerformanceMode::LowLatency);
        b.setSharingMode(SharingMode::Exclusive);

        b.setCallback(this);

        b.openStream(&stream_);

        link_manager_.UpdateMicrosPerSample(stream_->getSampleRate());

        stream_->setBufferSizeInFrames(stream_->getFramesPerBurst() * 2);
        stream_->requestStart();

        auto looper = make_unique<grannynorman::SamplePlayer>(mgr_, "Sounds/thinkloop.wav");
        sound_generators_.emplace_back(std::move(looper));

    }

    void AudioEngine::EmitEvent() {

        int midi_tick = link_manager_.GetMidiTick();

        Event ev(EventType::MIDI_TICK, midi_tick);

        if (midi_tick % PPBAR == 0) {
            ev.is_start_of_bar = true;
            //LinkData link_data = link_manager_.GetStatus();
           // __android_log_print(ANDROID_LOG_ERROR, "BAR", "LINK DATA peers:%d tempo:%f", link_data.num_peers, link_data.tempo);

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

    DataCallbackResult
    AudioEngine::onAudioReady(AudioStream *oboeStream, void *audio_data, int32_t num_frames) {

        link_manager_.UpdateFromAudioCallback(num_frames);

        float *data = static_cast<float *>(audio_data);
        for (int i = 0; i < num_frames; i++) {

            double output_val{0};
            for (const auto &sg: sound_generators_) {
                output_val += sg->Generate();
            }

            data[i] = output_val;


            if (link_manager_.IsMidiTick(i)) {
                EmitEvent();

            }
        }


        return
                DataCallbackResult::Continue;
    }

} // namespace
