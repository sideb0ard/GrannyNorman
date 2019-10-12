//
// Created by Thorsten Sideboard on 2019-10-12.
//

#ifndef GRANNYNORMAN_IRENDERABLEAUDIO_H
#define GRANNYNORMAN_IRENDERABLEAUDIO_H

#include <cstdint>
#include <string>
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdint"

class IRenderableAudio {

public:
    virtual ~IRenderableAudio() = default;
    virtual void renderAudio(float *audioData, int32_t numFrames) = 0;
};

#endif //GRANNYNORMAN_IRENDERABLEAUDIO_H
