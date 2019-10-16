//
// Created by Thorsten Sideboard on 2019-10-15.
//

#include <android/log.h>
#include "SamplePlayer.h"
#include "WavData.h"

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

} // namespace grannynorman