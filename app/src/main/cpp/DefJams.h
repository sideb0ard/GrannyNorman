//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_DEFJAMS_H
#define GRANNYNORMAN_DEFJAMS_H

#include <memory>



    using StereoValue = std::pair<double, double>;

    constexpr int PPQN = 960;       // pulses per quarter note
    constexpr int PPBAR = PPQN * 4; // pulses per Bar
    constexpr double MIDI_TICK_FRAC_OF_BEAT = 1. / PPQN;
    constexpr double DEFAULT_SAMPLE_RATE = 44100;
    constexpr double DEFAULT_BPM = 120;

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }





#endif // GRANNYNORMAN_DEFJAMS_H
