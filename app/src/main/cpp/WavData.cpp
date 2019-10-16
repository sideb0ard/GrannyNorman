//
// Created by Thorsten Sideboard on 2019-10-13.
//

#include "WavData.h"
#include "../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstring"
#include "oboe/src/common/OboeDebug.h"

namespace {
    void ConvertPCMArrayToFloat(int16_t *data, float *fdata, int length)
    {
        for (int i = 0; i < length; ++i){
            fdata[i] = data[i] / 32768.0;
        }
    }

    void ConvertFloatArrayToPCM(float *fdata, int16_t *data, int length)
    {
        for (int i = 0; i < length; ++i){
            data[i] = fdata[i] * 32768.0;
        }
    }
} // namespace

bool WavDataLoadFromAssetBuffer(WavData *wavData, unsigned char const *buffer) {

    // Format details from:
    // http://soundfile.sapp.org/doc/WaveFormat/

    int buffer_idx = 0;
    // unsigned char const *unsigned_buffer = (unsigned char const *)buffer;

    char scratch[5];
    scratch[4] = '\0';

    // ChunkID
    memcpy(scratch, &buffer[buffer_idx], 4);
    buffer_idx += 4;

    if (strncmp(scratch, "RIFF", 4) == 0) {

        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "CHUNKID::: %s", scratch);

        // ChunkSize
        int chunksize =
                (int) buffer[buffer_idx] | (int) buffer[buffer_idx + 1] << 8 |
                (int) buffer[buffer_idx + 2] << 16 |
                (int) buffer[buffer_idx + 3] << 24;

        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "CHUNSIZE::: %d", chunksize);
        for (int i = 0; i < 4; i++) {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "BYTE %d::: %d", i,
                                buffer[buffer_idx + i]);
        }
        buffer_idx += 4;

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
            int audio_format =
                    (unsigned) buffer[buffer_idx] | ((unsigned) buffer[buffer_idx + 1]) << 8;
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "FORMAT::: %d", audio_format);
            buffer_idx += 2;

            // NumChannels
            wavData->num_channels =
                    (unsigned) buffer[buffer_idx] | ((unsigned) buffer[buffer_idx + 1]) << 8;
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "NUM CHANNELS::: %d",
                                wavData->num_channels);
            buffer_idx += 2;

            // SampleRate

            wavData->sample_rate =
                    (int) buffer[buffer_idx] | (int) buffer[buffer_idx + 1] << 8 |
                    (int) buffer[buffer_idx + 2] << 16 |
                    (int) buffer[buffer_idx + 3] << 24;
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "SAMPLERATE::: %d",
                                wavData->sample_rate);
            for (int i = 0; i < 4; i++) {
                __android_log_print(ANDROID_LOG_ERROR, "WOOP", "BYTE %d::: %d", i,
                                    (unsigned int) buffer[buffer_idx + i]);
            }
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
            int data_size_bytes =
                    (int) buffer[buffer_idx] | (int) buffer[buffer_idx + 1] << 8 |
                    (int) buffer[buffer_idx + 2] << 16 |
                    (int) buffer[buffer_idx + 3] << 24;
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "DATA SIZE BYTES::: %d",
                                data_size_bytes);
            buffer_idx += 4;

            // DATA!
            wavData->data_len = data_size_bytes / sizeof(short);
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "DATA LEN::: %lu", wavData->data_len);
            wavData->data = new int16_t[wavData->data_len];
            memcpy(wavData->data, &buffer[buffer_idx], data_size_bytes);
            wavData->fdata = new float[wavData->data_len];
            ConvertPCMArrayToFloat(wavData->data, wavData->fdata, wavData->data_len);

            return true;


        } else {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "Err! RIFF file but not a wave %s",
                                scratch);
            return false;
        }
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "WOOP", "ERR! NOT A RIFF fike");
        return false;
    }
}