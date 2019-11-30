//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_SAMPLEPLAYER_H
#define GRANNYNORMAN_SAMPLEPLAYER_H

#include <android/asset_manager.h>
#include <string>
#include <array>

#include "DefJams.h"
#include "EnvelopeGenerator.h"
#include "Event.h"
#include "SoundGenerator.h"
#include "TimingData.h"
#include "WavData.h"


constexpr int max_grains = 1000;

constexpr int grain_duration_min_ms = 10;
constexpr int grain_duration_max_ms = 200;
constexpr int grain_duration_default_ms = 64;

constexpr int grains_per_second_min = 1;
constexpr int grains_per_second_max = 100;
constexpr int grains_per_second_default = 30;

constexpr int grain_spray_min_ms = 0;
constexpr int grain_spray_max_ms = 500;

constexpr int grain_fudge_min_ms = 0;
constexpr int grain_fudge_max_ms = 500;

enum class SELECTION_MODE {
    STATIC,
    RANDOM,
};

enum class LOOP_MODE {
    LOOP,
    STATIC,
    SMUDGE
};

enum class ENVELOPE_MODE {
    PARABOLIC,
    TRAPEZOIDAL,
    EXPONENTIAL_CURVE,
    LOGARITHMIC_CURVE,
};

struct SoundGrainInitParams {
    int sample_rate;
    int dur;
    int starting_idx;
    int attack_pct;
    int release_pct;
    bool reverse_mode;
    double pitch;
    int num_channels;
    int degrade_by;
    ENVELOPE_MODE envelope_mode;
    bool debug;
};

struct SoundGrain {

    SoundGrain() = default;

    SoundGrain(SoundGrainInitParams params);

    int grain_len_frames;
    int grain_counter_frames;
    int audiobuffer_num_channels;
    float audiobuffer_cur_pos;
    int audiobuffer_starting_idx;
//    float audiobuffer_pitch;
    float incr;

    int degrade_by;

    bool active;


    //// Envelope vars
    ENVELOPE_MODE envelope_mode;
    float amp;

    //EnvelopeGenerator env_;
    // Parabolic Env vars
    float slope;
    float curve;


    int attack_time_samples;
    int release_time_samples;
    int attack_to_sustain_boundary_sample_idx;
    int sustain_to_decay_boundary_sample_idx;
    float previous_amplitude;

    // Trapezoidal Env vars
    float amplitude_increment;

    // Exponential / Logarithmic
    float exp_min = 0.2;
    float exp_mul = 0;
    float exp_now = 0;


    bool reverse_mode;
    bool debug;
    int use_count{0};


};

class SamplePlayer : public SoundGenerator {

public:

    SamplePlayer(AAssetManager *mgr, std::string const &sample_name);

    StereoValue Generate(TimingData timing_data) override;

    void EventNotify(Event ev) override;

    void SetParam(std::string val_name, double val) override;

    float GetParam(std::string val_name) override;

    void Reset() override;

    void Random() override;


private:
    WavData sample_data_;
    int read_idx_{0};

    int num_active_grains_{0};
    int highest_grain_num_{0};
    int cur_grain_num_{0};

    std::array<SoundGrain, max_grains> grains_{};

    int granular_spray_ms_{0}; // random off-set from starting read index (ms)
    int granular_fudge_ms{0};     // random variation added to duration of grain (ms)
    int grain_duration_ms_{grain_duration_default_ms};
    int grains_per_sec_{grains_per_second_default};

    bool reverse_mode_{false};
    bool debug_{false};
    int degrade_by_{0};
    bool density_duration_sync_{true}; // keep duration and per_sec aligned
    double fill_factor_{3};         // used for density_duration_sync
    double grain_pitch_{1};

    int num_grains_per_looplen_{0};
    SELECTION_MODE selection_mode_{SELECTION_MODE::STATIC};
    ENVELOPE_MODE envelope_mode_{ENVELOPE_MODE::PARABOLIC};

    double envelope_taper_ratio_{0.5}; // 0.0...1.0


    int last_grain_launched_frame_tick_{0};
    int grain_attack_time_pct_{25};
    int grain_release_time_pct_{25};

    LOOP_MODE loop_mode_{LOOP_MODE::LOOP}; // enums above - LOOP, SMUDGE, STATIC
    double loop_len_{1};        // bars
    int loop_counter_{-1};

    int size_of_sixteenth_in_frames_;

    bool scramble_mode_{false};
    bool scramble_pending_{false};
    int scramble_diff_{0};

    bool stutter_mode_{false};
    bool stutter_pending_{false};
    int stutter_idx_{0};

    bool log_env_val{true};

private:
    int CalculateGrainSpacing(TimingData timing_data);

    int GetAvailableGrainNumber();

    void SetGrain(SoundGrain &grain, SoundGrainInitParams params);

    StereoValue SoundGrainGenerate(int i);

    void SetGrainsPerSecond(double val);

    void SetGrainDuration(double val);

    void SetGrainFudge(double val);

    void SetGrainSpray(double val);

    void SetGrainIndex(int val);
    int GetGrainIndex();

    void SetGranularMode(int val);

    void SetEnvelopeMode(int val);

    void SetStutterMode();
    void SetScrambleMode();


};


#endif //GRANNYNORMAN_SAMPLEPLAYER_H
