//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_SAMPLEPLAYER_H
#define GRANNYNORMAN_SAMPLEPLAYER_H

#include <android/asset_manager.h>
#include <string>
#include <array>
#include "SoundGenerator.h"
#include "WavData.h"
#include "EnvelopeGenerator.h"


namespace grannynorman {

    constexpr int max_grains = 1000;

    enum class SELECTION_MODE {
        STATIC,
        RANDOM,
    };

    enum class LOOP_MODE {
        LOOP,
        STATIC,
        SMUDGE
    };

    struct SoundGrainInitParams {
        int dur;
        int starting_idx;
        int attack_pct;
        int release_pct;
        bool reverse_mode;
        double pitch;
        int num_channels;
        int degrade_by;
        bool debug;
    };

    class SoundGrain {

    public:
        SoundGrain() = default;

        SoundGrain(SoundGrainInitParams params);

    private:
        int grain_len_frames;
        int grain_counter_frames;
        int audiobuffer_num;
        int audiobuffer_start_idx;
        int audiobuffer_num_channels;
        double audiobuffer_cur_pos;
        double audiobuffer_pitch;
        double incr;

        int degrade_by;

        int attack_time_pct; // percent of grain_len_frames
        int attack_time_samples;
        int release_time_pct; // percent of grain_len_frames
        int release_time_samples;
        bool active;
        double amp;
        double slope;
        double curve;
        bool reverse_mode;
        bool debug;
        //EnvelopeGenerator env_;

    };

    class SamplePlayer : public SoundGenerator {

    public:

        SamplePlayer(AAssetManager *mgr, std::string sample_name);

        double Generate() override;

        void EventNotify(Event ev) override;


    private:
        WavData sample_data_;
        int read_idx_{0};
        bool active_{false};

        int num_active_grains_{0};
        int highest_grain_num_{0};
        int cur_grain_num_{0};

        std::array<SoundGrain, max_grains> grains_{};

        int granular_spray_frames_{441}; // random off-set from starting read_idx
        int quasi_grain_fudge_{220};     // random variation from length of grain
        int grain_duration_ms_{100};
        int grains_per_sec_{30};
        bool density_duration_sync_{true}; // keep duration and per_sec aligned
        double fill_factor_{3};         // used for density_duration_sync
        double grain_pitch_{1};

        int num_grains_per_looplen_;
        SELECTION_MODE selection_mode_{SELECTION_MODE::STATIC};

        double envelope_taper_ratio_{0.5}; // 0.0...1.0


        int last_grain_launched_sample_time_{0};
        int grain_attack_time_pct_{15};
        int grain_release_time_pct_{15};

        LOOP_MODE loop_mode_{LOOP_MODE::LOOP}; // enums above - LOOP, STEP, STATIC
        double loop_len_{1};        // bars
        int loop_counter_{-1};

    };

}


#endif //GRANNYNORMAN_SAMPLEPLAYER_H
