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
    osc_.setSampleRate(stream_->getSampleRate());

    stream_->setBufferSizeInFrames(stream_->getFramesPerBurst() * 2);

    stream_->requestStart();

}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    osc_.renderAudio(static_cast<float *>(audioData), numFrames);

    return DataCallbackResult::Continue;
}

void AudioEngine::tap(bool b) {
    osc_.setWaveOn(b);
}

void AudioEngine::setFrequency(float d) {
    osc_.setFrequency(d);
}

bool AudioEngine::LoadSamples(AAssetManager *mgr) {

    char const *think_sample = "thinkloop.wav";

    AAssetDir *assetDir = AAssetManager_openDir(mgr, "Sounds");
    const char *filename;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
        __android_log_print(ANDROID_LOG_ERROR, "DJOBBIO", "File:%s", filename);
        if (strncmp(filename, think_sample, strlen(think_sample)) == 0) {
            __android_log_print(ANDROID_LOG_ERROR, "DJOBBIO", "COMPARED WELL!");

            AAsset * think_wav = AAssetManager_open(mgr, "Sounds/thinkloop.wav", AASSET_MODE_BUFFER);
            if (think_wav) {
                __android_log_print(ANDROID_LOG_ERROR, "WOOP", "OPENED THINK: %d", AAsset_getLength(think_wav));
            }
        }
    }

    AAssetDir_close(assetDir);

    return true;
}
