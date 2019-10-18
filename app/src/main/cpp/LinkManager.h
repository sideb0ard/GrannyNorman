//
// Created by Thorsten Sideboard on 2019-10-14.
//

#ifndef GRANNYNORMAN_LINKMANAGER_H
#define GRANNYNORMAN_LINKMANAGER_H

#include <ableton/Link.hpp>
#include <ableton/link/HostTimeFilter.hpp>
#include <ableton/platforms/Config.hpp>
#include "TimingData.h"




    class LinkManager {
    public:
        LinkManager();

        void UpdateFromAudioCallback(int num_frames);

        bool IsMidiTick(int frame_num);

        TimingData GetTimingData();

        int GetMidiTick();

        void SetSampleRate(int sample_rate);


    private:
        void IncMidi(double beat_time);

        void UpdateMicrosPerSample(int sample_rate);


    private:
        ableton::Link link_;
        ableton::Link::SessionState session_state_;
        ableton::link::HostTimeFilter<ableton::link::platform::Clock> host_time_filter_;

        std::chrono::microseconds hosttime_{0}; // also updated every callback
        std::chrono::microseconds buffer_begin_at_output_{0};

        std::chrono::microseconds output_latency_{0};

        bool reset_beat_time_{false};

        double quantum_{4.};
        double requested_tempo_{0.};

        double micros_per_sample{0};

        int frame_counter_{0};

        int midi_tick_{0};
        double time_of_next_midi_tick_{0};

        int sixteenth_note_tick_{0};

        bool is_midi_tick_{false};
        bool is_thirtysecond_{false};
        bool is_twentyfourth_{false};
        bool is_sixteenth_{false};
        bool is_twelth_{false};
        bool is_eighth_{false};
        bool is_sixth_{false};
        bool is_quarter_{false};
        bool is_third_{false};
        bool is_start_of_bar_{false};

        double bpm_{0};
        int sample_rate_{0};

        int frames_per_midi_tick_{0};
        double ms_per_midi_tick_{0};

        int loop_len_in_frames_{0};
        int loop_len_in_ticks_{0};


    };


#endif //GRANNYNORMAN_LINKMANAGER_H
