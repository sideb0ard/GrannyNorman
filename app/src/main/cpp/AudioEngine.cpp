//
// Created by Thorsten Sideboard on 2019-10-12.
//

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

    b.openStream(&stream);

    osc.setAmplitude(0.3);
    osc.setFrequency(80.);
    osc.setSampleRate(stream->getSampleRate());

    stream->setBufferSizeInFrames(stream->getFramesPerBurst() * 2);

    stream->requestStart();

}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    osc.renderAudio(static_cast<float *>(audioData), numFrames);

    return DataCallbackResult::Continue;
}

void AudioEngine::tap(bool b) {
    osc.setWaveOn(b);
}

void AudioEngine::setFrequency(float d) {
    osc.setFrequency(d);

}
