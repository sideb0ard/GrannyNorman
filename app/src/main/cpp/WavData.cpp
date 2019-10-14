//
// Created by Thorsten Sideboard on 2019-10-13.
//

#include "WavData.h"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstring"
#include "oboe/src/common/OboeDebug.h"

void WavDataLoadFromAssetBuffer(WavData *wavData, char const *buffer) {

    // Format details from:
    // http://soundfile.sapp.org/doc/WaveFormat/

    int buffer_idx = 0;

    char scratch[5];
    scratch[4] = '\0';

    // ChunkID
    memcpy(scratch, &buffer[buffer_idx], 4);
    buffer_idx += 4;

    if (strncmp(scratch, "RIFF", 4) == 0) {

        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "CHUNKID::: %s", scratch);

        // ChunkSize
        int chunksize =
                (unsigned) buffer[0] | ((unsigned) buffer[1]) << 8 | ((unsigned) buffer[2]) << 16 |
                ((unsigned) buffer[3]) << 24;
        buffer_idx += 4;

        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "CHUNSIZE::: %d", chunksize);

//            ss.read(scratch, 4);
//            unsigned int overall_size =
//                    scratch[0] | scratch[1] << 8 | scratch[2] << 16 | scratch[3] << 24;
//            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "Overall Size:%lu", overall_size);

        // Format
        memcpy(scratch, &buffer[buffer_idx], 4);
        buffer_idx += 4;
        scratch[4] = '\0';

        if (strncmp(scratch, "WAVE", 4) == 0) {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "BOOM!! RIFF and WAVE");

            //Subchunk1ID
            buffer_idx += 4;

            // Subchunk1Size
            buffer_idx += 4;

            // AudioFormat
            buffer_idx += 2;

            // NumChannels
            buffer_idx += 2;

            // SampleRate
            buffer_idx += 4;

            // ByteRate
            buffer_idx += 4;

            // BlockAlign
            buffer_idx += 2;

            // BitsPerSample
            buffer_idx += 2;

            // SubChunk2ID - Contains the letters "data"
            //                               (0x64617461 big-endian form).
            memcpy(scratch, &buffer[buffer_idx], 4);
            buffer_idx += 4;
            scratch[4] = '\0';
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "IS data?? %s",
                                scratch);

            // SubChunk2Size -  == NumSamples * NumChannels * BitsPerSample/8
            //                               This is the number of bytes in the data.
            //                               You can also think of this as the size
            //                               of the read of the subchunk following this
            //                               number.
            buffer_idx += 4;

            // DATA!
            // int data_size = scratch[0] | scratch[1] << 8 | scratch[2] << 16 | scratch[3] << 24;
            // allocate SubChunk2Size memory for data
            // think_sample_.data = new();
            // ss.read(think_sample_.data, data_size);


        } else {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "Err! RIFF file but not a wave %s",
                                scratch);
        }
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "ERR! NOT A RIFF fike");
    }
}