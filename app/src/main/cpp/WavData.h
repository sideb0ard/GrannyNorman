//
// Created by Thorsten Sideboard on 2019-10-13.
//

#ifndef GRANNYNORMAN_WAVDATA_H
#define GRANNYNORMAN_WAVDATA_H

#include <cstdint>



    struct WavData {
        int16_t *data = nullptr;
        float *fdata = nullptr;
        unsigned long data_len{0};
        int num_channels{0};
        int sample_rate{0};
        int precision{0};
    };

    bool WavDataLoadFromAssetBuffer(WavData *wavData, unsigned char const *buffer);



#endif // GRANNYNORMAN_WAVDATA_H
