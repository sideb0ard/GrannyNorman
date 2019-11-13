//
// Created by Thorsten Sideboard on 2019-10-17.
//

#ifndef GRANNYNORMAN_TIMINGDATA_H
#define GRANNYNORMAN_TIMINGDATA_H



    struct TimingData {
        // bunch of timing data useful to sound generators ///////

        int bpm{120}; // TODO wirein

        int sample_rate{0};

        int frame_tick{0}; // frame count since start.
        int midi_tick{0}; // midi count since start.

        int sixteenth_note_tick{0};

        int frames_per_midi_tick{0};
        double ms_per_midi_tick{0};

        unsigned int loop_len_in_frames{0};
        unsigned int loop_len_in_ticks{0};

    };


#endif //GRANNYNORMAN_TIMINGDATA_H
