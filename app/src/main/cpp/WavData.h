//
// Created by Thorsten Sideboard on 2019-10-13.
//

#ifndef GRANNYNORMAN_WAVDATA_H
#define GRANNYNORMAN_WAVDATA_H

// taken from http://rogerchansdigitalworld.blogspot.com/2010/05/how-to-read-wav-format-file-in-c.html

struct WavData {
    short* data= nullptr;
    unsigned long size{0};
};

void LoadWavData()

#endif //GRANNYNORMAN_WAVDATA_H
