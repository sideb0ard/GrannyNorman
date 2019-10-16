//
// Created by Thorsten Sideboard on 2019-10-12.
//

#include <android/log.h>
#include <memory>

#include "AudioEngine.h"
#include "defjams.h"

#include "oboe/include/oboe/Definitions.h"
#include "oboe/include/oboe/AudioStreamBuilder.h"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/strstream"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdint"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/aaudio/AAudio.h"
#include "SamplePlayer.h"


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
    __android_log_print(ANDROID_LOG_ERROR, "WOOP", "EVENT BEEP!");
}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audio_data, int32_t num_frames) {

    link_manager_.UpdateFromAudioCallback(num_frames);

    float *data = static_cast<float *>(audio_data);
    for (int i = 0; i < num_frames; i++) {

        double output_val{0};
        for (const auto & sg: sound_generators_){
            output_val += sg->Generate();
        }

        data[i] = output_val;



        if (link_manager_.IsMidiTick(i)) {
            int midi_tick = link_manager_.GetMidiTick();
            if (midi_tick % PPBAR == 0) {
                //__android_log_print(ANDROID_LOG_ERROR, "WOOP", "START OF BAR! %d", midi_tick);
            }
            // EmitEvent();
        }
    }


    return
            DataCallbackResult::Continue;
}


