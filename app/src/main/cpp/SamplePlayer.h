//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_SAMPLEPLAYER_H
#define GRANNYNORMAN_SAMPLEPLAYER_H

#include <android/asset_manager.h>
#include <string>
#include "SoundGenerator.h"
#include "WavData.h"


namespace grannynorman {


    class SamplePlayer : public SoundGenerator {

    public:

        SamplePlayer(AAssetManager *mgr, std::string sample_name);

        double Generate() override;


    private:
        WavData sample_data_;
        int read_idx_{0};
        bool active_{false};

    };

}


#endif //GRANNYNORMAN_SAMPLEPLAYER_H
