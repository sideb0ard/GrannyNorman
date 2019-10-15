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

    osc_.setAmplitude(0.5);
    osc_.setFrequency(80.);
    __android_log_print(ANDROID_LOG_ERROR, "WOOP", "SAMPLE RATE: %d", stream_->getSampleRate());
    osc_.setSampleRate(stream_->getSampleRate());

    stream_->setBufferSizeInFrames(stream_->getFramesPerBurst() * 2);

    stream_->requestStart();

}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {

//    osc_.renderAudio(static_cast<float *>(audioData), numFrames);

    //AudioFormat dataFormat = oboeStream->getDataFormat();


    float *data = static_cast<float *>(audioData);
    for (int i = 0; i < numFrames; i++) {


        data[i] = think_sample_.fdata[read_idx++];

        if (read_idx == think_sample_.data_len) read_idx = 0;
    }


    return DataCallbackResult::Continue;
}

void AudioEngine::tap(bool b) {
    osc_.setWaveOn(b);
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
    }


    unsigned char const *asset_buffer = static_cast<unsigned char const *>(AAsset_getBuffer(
            think_wav));
    if (asset_buffer) {

        WavDataLoadFromAssetBuffer(&think_sample_, asset_buffer);

    }


}
