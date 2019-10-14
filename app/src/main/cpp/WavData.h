//
// Created by Thorsten Sideboard on 2019-10-13.
//

#ifndef GRANNYNORMAN_WAVDATA_H
#define GRANNYNORMAN_WAVDATA_H

struct WavData {
    short *data = nullptr;
    unsigned long data_size{0};
    int num_channels{0};
    int sample_rate{0};
    int precision{0};
};

void WavDataLoadFromAssetBuffer(WavData *wavData, char const *buffer );

// void LoadWavData()

#endif //GRANNYNORMAN_WAVDATA_H
