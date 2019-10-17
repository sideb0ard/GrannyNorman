//
// Created by Thorsten Sideboard on 2019-10-14.
//

#ifndef GRANNYNORMAN_LINKMANAGER_H
#define GRANNYNORMAN_LINKMANAGER_H

#include <ableton/Link.hpp>
#include <ableton/link/HostTimeFilter.hpp>
#include <ableton/platforms/Config.hpp>

namespace grannynorman {

    class LinkManager {
    public:
        LinkManager();

        double getTempo();

        double getPhase();

        void enable();

        void UpdateFromAudioCallback(int num_frames);

        void UpdateMicrosPerSample(int sample_rate);

        bool IsMidiTick(int frame_num);

        int GetMidiTick();


    private:
        ableton::Link link_;
        ableton::Link::SessionState session_state_;
        ableton::link::HostTimeFilter<ableton::link::platform::Clock> host_time_filter_;

        std::chrono::microseconds hosttime_{0}; // also updated every callback
        std::chrono::microseconds buffer_begin_at_output_{0};

        std::chrono::microseconds output_latency_{0};

        bool reset_beat_time_{false};

        int frame_counter_{0};

        int midi_tick_{0};
        bool is_midi_tick_{false};
        double time_of_next_midi_tick_;

        double quantum_{4.};
        double requested_tempo_{0.};

        double micros_per_sample{0};

    private:
        void IncMidi(double beat_time);


    };

} // namespace

#endif //GRANNYNORMAN_LINKMANAGER_H
