//
// Created by Thorsten Sideboard on 2019-10-12.
//

#include <android/log.h>
#include "AudioEngine.h"
#include "oboe/include/oboe/Definitions.h"
#include "oboe/include/oboe/AudioStreamBuilder.h"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/strstream"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdint"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/aaudio/AAudio.h"


void AudioEngine::start() {

    AudioStreamBuilder b;

    b.setFormat(AudioFormat::Float);
    b.setChannelCount(1);

    b.setPerformanceMode(PerformanceMode::LowLatency);
    b.setSharingMode(SharingMode::Exclusive);

    b.setCallback(this);

    b.openStream(&stream_);

//    osc_.setAmplitude(0.5);
//    osc_.setFrequency(80.);
//    __android_log_print(ANDROID_LOG_ERROR, "WOOP", "SAMPLE RATE: %d", stream_->getSampleRate());
//    osc_.setSampleRate(stream_->getSampleRate());

    link_manager_.UpdateMicrosPerSample(stream_->getSampleRate());

    stream_->setBufferSizeInFrames(stream_->getFramesPerBurst() * 2);
    stream_->requestStart();

    active_ = true;

}

void AudioEngine::EmitEvent()
{
    __android_log_print(ANDROID_LOG_ERROR, "WOOP", "EVENT BEEP!");
}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audio_data, int32_t num_frames) {

    link_manager_.UpdateFromAudioCallback(num_frames);

    if (active_) {
        float *data = static_cast<float *>(audio_data);
        for (int i = 0; i < num_frames; i++) {

            data[i] = think_sample_.fdata[read_idx_++];
            if (read_idx_ == think_sample_.data_len) read_idx_ = 0;

            if (link_manager_.IsMidiTick(i)) {
                // EmitEvent();
            }
        }
    } else {
        memset(audio_data, 0, sizeof(float) * num_frames);
    }

    return
            DataCallbackResult::Continue;
}

void AudioEngine::tap(bool b) {
    active_ = b;
}

void AudioEngine::setFrequency(float d) {
    osc_.setFrequency(d);
}

void AudioEngine::LoadSamples(AAssetManager *mgr) {

    AAsset *think_wav = AAssetManager_open(mgr, "Sounds/thinkloop.wav", AASSET_MODE_BUFFER);
    int file_size{0};
    if (think_wav) {
        file_size = AAsset_getLength(think_wav);
        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "OPENED THUNK: %d", file_size);

        unsigned char const *asset_buffer = static_cast<unsigned char const *>(AAsset_getBuffer(
                think_wav));
        if (asset_buffer) {
            WavDataLoadFromAssetBuffer(&think_sample_, asset_buffer);
        }
    }
}
