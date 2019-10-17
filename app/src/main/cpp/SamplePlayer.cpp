//
// Created by Thorsten Sideboard on 2019-10-15.
//

#include <android/log.h>
#include "SamplePlayer.h"
#include "WavData.h"
#include "defjams.h"

namespace grannynorman {

    SamplePlayer::SamplePlayer(AAssetManager *mgr, std::string sample_name) {

        AAsset *wav = AAssetManager_open(mgr, sample_name.c_str(), AASSET_MODE_BUFFER);

        if (wav) {

            unsigned char const *asset_buffer = static_cast<unsigned char const *>(AAsset_getBuffer(
                    wav));
            if (asset_buffer) {
                if (WavDataLoadFromAssetBuffer(&sample_data_, asset_buffer)) {
                    active_ = true;
                }
            }
        }
    }

    double SamplePlayer::Generate() {
        double return_val;
        if (active_) {
            return_val = sample_data_.fdata[read_idx_++];
            if (read_idx_ == sample_data_.data_len) read_idx_ = 0;
        } else {
            return_val = 0;
        }

        return return_val;
    }

    void SamplePlayer::EventNotify(Event ev) {



        int relative_midi_tick = ev.midi_tick % PPBAR;
        double percent_of_loop = (double)relative_midi_tick / PPBAR;
        double new_read_idx = percent_of_loop * sample_data_.data_len;
        if (percent_of_loop < 0 || percent_of_loop > 1) {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "YOUCH!! REL MIDI:%d PCT:%f NEW READ IDX:%f",
                                relative_midi_tick, percent_of_loop,
                                new_read_idx);
        }
        read_idx_ = new_read_idx;

        if (ev.is_start_of_bar) {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP", "START OF LOOPO:%d READ IDX:%d",
                                ev.midi_tick % PPBAR, read_idx_);
        }


    }

} // namespace grannynorman