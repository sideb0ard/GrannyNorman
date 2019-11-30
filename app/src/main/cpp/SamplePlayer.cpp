//
// Created by Thorsten Sideboard on 2019-10-15.
//

#include "SamplePlayer.h"
#include "DefJams.h"
#include "WavData.h"
#include <android/log.h>
#include <cmath>

namespace {
    void CheckIndex(int *index, int buffer_len) {
        while (*index < 0.0)
            *index += buffer_len;
        while (*index >= buffer_len)
            *index -= buffer_len;
    }

    float LinTerp(float x1, float x2, float y1, float y2, float x) {
        float denom = x2 - x1;
        if (denom == 0)
            return y1; // should not ever happen

        // calculate decimal position of x
        float dx = (x - x1) / (x2 - x1);

        // use weighted sum method of interpolating
        float result = dx * y2 + (1 - dx) * y1;

        return result;
    }
}


void SamplePlayer::Reset() {

    grain_duration_ms_ = grain_duration_default_ms;
    grains_per_sec_ = grains_per_second_default;
    granular_fudge_ms = 0;
    granular_spray_ms_ = 0;
    loop_mode_ = LOOP_MODE::LOOP;
}

void SamplePlayer::Random() {

    int read_idx_percent = rand() % 100;
    SetGrainIndex(read_idx_percent);

    grain_duration_ms_ =
            (rand() % (grain_duration_max_ms - grain_duration_min_ms)) + grain_duration_min_ms;
    grains_per_sec_ =
            (rand() % 20) + grains_per_second_min;
    granular_fudge_ms = rand() % grain_fudge_max_ms;
    granular_spray_ms_ = rand() % grain_spray_max_ms;
    if (rand() % 100 > 50)
        loop_mode_ = LOOP_MODE::LOOP;
    else
        loop_mode_ = LOOP_MODE::SMUDGE;

    __android_log_print(ANDROID_LOG_ERROR, "ENV", "EnvMode:%d", envelope_mode_);
}


SamplePlayer::SamplePlayer(AAssetManager *mgr, std::string const &sample_name) {

    AAsset *wav =
            AAssetManager_open(mgr, sample_name.c_str(), AASSET_MODE_BUFFER);

    if (wav) {

        auto *asset_buffer =
                static_cast<unsigned char const *>(AAsset_getBuffer(wav));
        if (asset_buffer) {
            if (WavDataLoadFromAssetBuffer(&sample_data_, asset_buffer)) {
                // active_ = true;
            }
        }
    }

    Reset();
}

StereoValue SamplePlayer::Generate(TimingData timing_data) {

    if (!started_)
        return std::make_pair<double, double>(0, 0);

    // STEP 1 - calculate if we should launch a new grain
    int spacing = CalculateGrainSpacing(timing_data);
    if (timing_data.frame_tick >
        last_grain_launched_frame_tick_ + spacing) // new grain time
    {
        last_grain_launched_frame_tick_ = timing_data.frame_tick;

        cur_grain_num_ = GetAvailableGrainNumber();
//        if (cur_grain_num_ == 0 && log_env_val)
//            __android_log_print(ANDROID_LOG_ERROR, "ENV",
//                                "Launching Grain 0");

        float samples_per_ms = timing_data.sample_rate / 1000.;
        int duration_ms = grain_duration_ms_;
        if (granular_fudge_ms != 0)
            duration_ms += rand() % (int) granular_fudge_ms;
        int duration_frames = duration_ms * samples_per_ms;

        int grain_idx = read_idx_;
        if (selection_mode_ == SELECTION_MODE::RANDOM)
            grain_idx = rand() % (sample_data_.data_len -
                                  (duration_frames * sample_data_.num_channels));

        if (granular_spray_ms_ > 0)
            grain_idx += rand() % (int) (granular_spray_ms_ * samples_per_ms);

        int attack_time_pct = grain_attack_time_pct_;
        int release_time_pct = grain_release_time_pct_;

        SoundGrainInitParams params = {
                .dur = duration_frames,
                .sample_rate = timing_data.sample_rate,
                .starting_idx = grain_idx,
                .attack_pct = attack_time_pct,
                .release_pct = release_time_pct,
                .reverse_mode = reverse_mode_,
                .pitch = grain_pitch_,
                .num_channels =
                sample_data_.num_channels,
                .degrade_by = degrade_by_,
                .debug = debug_,
                .envelope_mode = envelope_mode_};

        if (cur_grain_num_ == 0)
            params.debug = 1;

        SetGrain(grains_[cur_grain_num_], params);
        // num_active_grains_ = CountActiveGrains();
    }

    // STEP 2 - gather values for all active grains

    double left_accumulator = 0, right_accumulator = 0;

    for (int i = 0; i < highest_grain_num_; i++) {

        StereoValue tmp = SoundGrainGenerate(i);
        left_accumulator += tmp.first;
        right_accumulator += tmp.second;
    }

    return std::make_pair(left_accumulator, right_accumulator);
}

void SamplePlayer::EventNotify(Event ev) {


    if (ev.is_start_of_bar && !started_) {
        started_ = true;
        __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                            "START OF LOOPO:%d READ IDX:%d",
                            ev.midi_tick % PPBAR, read_idx_);
    }

    if (ev.is_start_of_bar) {
        if (scramble_pending_) {
            __android_log_print(ANDROID_LOG_ERROR, "YAS!",
                                "sCRRRRAMMMBLE!");
            scramble_mode_ = true;
            scramble_pending_ = false;
        } else
            scramble_mode_ = false;

        if (stutter_pending_) {
            __android_log_print(ANDROID_LOG_ERROR, "YAS!",
                                "STUUUUTTTUTUUTUTUTUTUTUER");
            stutter_mode_ = true;
            stutter_pending_ = false;
        } else
            stutter_mode_ = false;
    }

    if (loop_mode_ == LOOP_MODE::LOOP) {
        int relative_midi_tick = ev.midi_tick % PPBAR;
        double percent_of_loop = (double) relative_midi_tick / PPBAR;
        double new_read_idx = percent_of_loop * sample_data_.data_len;
        if (percent_of_loop < 0 || percent_of_loop > 1) {
            __android_log_print(ANDROID_LOG_ERROR, "WOOP",
                                "YOUCH!! REL MIDI:%d PCT:%f NEW READ IDX:%f",
                                relative_midi_tick, percent_of_loop, new_read_idx);
        }
        if (scramble_mode_) {
            read_idx_ =
                    new_read_idx + (scramble_diff_ * sample_data_.size_of_sixteenth_in_frames);
        } else if (stutter_mode_) {
            int cur_sixteenth = ev.timing_data.sixteenth_note_tick % 16;
            int rel_pos_within_a_sixteenth =
                    new_read_idx - (cur_sixteenth * sample_data_.size_of_sixteenth_in_frames);
            read_idx_ =
                    (stutter_idx_ * sample_data_.size_of_sixteenth_in_frames) +
                    rel_pos_within_a_sixteenth;
        } else
            read_idx_ = new_read_idx;
    }

    if (ev.is_sixteenth) {
        if (scramble_mode_) {
            int cur_sixteenth = ev.timing_data.sixteenth_note_tick % 16;
            if (cur_sixteenth % 2 != 0) {
                int randy = rand() % 100;
                if (randy < 25)
                    scramble_diff_ = 3 - cur_sixteenth;
                else if (randy < 50)
                    scramble_diff_ = 4 - cur_sixteenth;
                else if (randy < 75)
                    scramble_diff_ = 7 - cur_sixteenth;
            }
        }

        if (stutter_mode_) {
            if (rand() % 100 > 75)
                stutter_idx_++;
            if (stutter_idx_ == 16)
                stutter_idx_ = 0;
        }
    }
}

int SamplePlayer::CalculateGrainSpacing(TimingData timing_data) {
    double looplen_in_seconds =
            timing_data.loop_len_in_frames / (double) timing_data.sample_rate;
    num_grains_per_looplen_ = looplen_in_seconds * grains_per_sec_;
    if (num_grains_per_looplen_ == 0) {
        num_grains_per_looplen_ = 2; // whoops! dn't wanna div by 0 below
    }
    int spacing = timing_data.loop_len_in_frames / num_grains_per_looplen_;

    return spacing;
}

int SamplePlayer::GetAvailableGrainNumber() {
    int idx = 0;
    while (idx < max_grains) {
        if (!grains_[idx].active) {
            if (idx > highest_grain_num_)
                highest_grain_num_ = idx;
            return idx;
        }
        idx++;
    }
    printf("WOW - NO GRAINS TO BE FOUND IN %d attempts\n", idx);
    return 0;
}

void SamplePlayer::SetGrain(SoundGrain &grain, SoundGrainInitParams params) {
    grain.grain_len_frames = params.dur;
    grain.grain_counter_frames = 0;
    grain.audiobuffer_num_channels = params.num_channels;
    grain.audiobuffer_starting_idx = params.starting_idx;
    grain.audiobuffer_cur_pos = grain.audiobuffer_starting_idx;
    grain.incr = params.pitch;

    grain.degrade_by = params.degrade_by;
    grain.debug = params.debug;

    grain.reverse_mode = params.reverse_mode;
    if (params.reverse_mode) {
        grain.audiobuffer_cur_pos =
                params.starting_idx + (params.dur * params.num_channels) - 1;
        grain.incr = -1.0 * params.pitch;
    }

    grain.audiobuffer_cur_pos = params.starting_idx;
    grain.incr = params.pitch;

    grain.envelope_mode = params.envelope_mode;
    grain.amp = 0;
    grain.use_count++;

    grain.attack_time_samples = params.dur / 100. * params.attack_pct;
    grain.attack_to_sustain_boundary_sample_idx = grain.attack_time_samples;
    grain.release_time_samples = params.dur / 100. * params.release_pct;
    grain.sustain_to_decay_boundary_sample_idx = params.dur - grain.release_time_samples;


    switch (grain.envelope_mode) {
        case (ENVELOPE_MODE::PARABOLIC): {
            double rdur = 1.0 / grain.grain_len_frames;
            double rdur2 = rdur * rdur;
            grain.slope = 4.0 * 1.0 * (rdur - rdur2);
            grain.curve = -8.0 * 1.0 * rdur2;
            break;
        }
        case (ENVELOPE_MODE::TRAPEZOIDAL):
            grain.amplitude_increment = 1.0 / grain.attack_time_samples;
            break;
        case (ENVELOPE_MODE::EXPONENTIAL_CURVE):
            grain.exp_mul = pow((grain.exp_min + 1.0) / grain.exp_min,
                                1.0 / grain.attack_time_samples);
            grain.exp_now = grain.exp_min;
            break;
        case (ENVELOPE_MODE::LOGARITHMIC_CURVE):
            grain.exp_mul = pow(grain.exp_min / (grain.exp_min + 1),
                                1.0 / grain.attack_time_samples);
            grain.exp_now = grain.exp_min + 1;
            break;

    }

//    if (grain.debug) {
//        __android_log_print(ANDROID_LOG_ERROR, "DEBUG0",
//                            "GRAIN! len_frames:%d counter:%d channels:%d cur_pos:%f incr:%f degrade_by:%d active:%d env_mode:%d amp:%f",
//                            grain.grain_len_frames, grain.grain_counter_frames,
//                            grain.audiobuffer_num_channels, grain.audiobuffer_cur_pos, grain.incr,
//                            grain.degrade_by, grain.active, grain.envelope_mode, grain.amp);
//    }

    grain.active = true;
}

std::pair<double, double> SamplePlayer::SoundGrainGenerate(int i) {
    SoundGrain &grain = grains_[i];
    std::pair<double, double> out = {0., 0.};

    if (!grain.active)
        return out;

    if (grain.degrade_by > 0) {
        if (rand() % 100 < grain.degrade_by)
            return out;
    }

    int num_channels = grain.audiobuffer_num_channels;

    int read_idx = (int) grain.audiobuffer_cur_pos;
    double frac = grain.audiobuffer_cur_pos - read_idx;
    CheckIndex(&read_idx, sample_data_.data_len);

    if (num_channels == 1) {
        int read_next_idx = read_idx + 1;
        CheckIndex(&read_next_idx, sample_data_.data_len);
        out.first = LinTerp(0, 1, sample_data_.fdata[read_idx],
                            sample_data_.fdata[read_next_idx], frac);
        out.second = out.first;
    } else if (num_channels == 2) {
        int read_next_idx = read_idx + 2;
        CheckIndex(&read_next_idx, sample_data_.data_len);
        out.first = LinTerp(0, 1, sample_data_.fdata[read_idx],
                            sample_data_.fdata[read_next_idx], frac);

        int read_idx_right = read_idx + 1;
        CheckIndex(&read_idx_right, sample_data_.data_len);
        int read_next_idx_right = read_idx_right + 2;
        CheckIndex(&read_next_idx_right, sample_data_.data_len);
        out.second = LinTerp(0, 1, sample_data_.fdata[read_idx_right],
                             sample_data_.fdata[read_next_idx_right], frac);
    }

    grain.audiobuffer_cur_pos += (grain.incr * num_channels);

    grain.grain_counter_frames++;
    if (grain.grain_counter_frames > grain.grain_len_frames) {
        grain.active = false;
//        if (i == 0)
//            log_env_val = false;
    }

    out.first *= grain.amp;
    out.second *= grain.amp;

    switch (grain.envelope_mode) {
        case (ENVELOPE_MODE::PARABOLIC):
            grain.amp = grain.amp + grain.slope;
            grain.slope = grain.slope + grain.curve;
            if (grain.amp < 0)
                grain.amp = 0;
            if (grain.amp > 1.0)
                grain.amp = 1.0;
            break;
        case (ENVELOPE_MODE::TRAPEZOIDAL):
            if (grain.grain_counter_frames < grain.attack_to_sustain_boundary_sample_idx)
                grain.amp += grain.amplitude_increment;
            else if (grain.grain_counter_frames == grain.attack_to_sustain_boundary_sample_idx)
                grain.amp = 1.0;
            else if (grain.grain_counter_frames == grain.sustain_to_decay_boundary_sample_idx)
                grain.amplitude_increment = -1. / grain.release_time_samples;
            break;
        case (ENVELOPE_MODE::EXPONENTIAL_CURVE):
            if (grain.grain_counter_frames < grain.attack_to_sustain_boundary_sample_idx ||
                grain.grain_counter_frames > grain.sustain_to_decay_boundary_sample_idx) {
                grain.exp_now *= grain.exp_mul;
                grain.amp = (grain.exp_now - grain.exp_min);
            } else if (grain.grain_counter_frames == grain.attack_to_sustain_boundary_sample_idx) {
                grain.amp = 1.;
            } else if (grain.grain_counter_frames == grain.sustain_to_decay_boundary_sample_idx) {
                grain.exp_now = 1 + grain.exp_min;
                grain.exp_mul =
                        pow(grain.exp_min / (1 + grain.exp_min), 1 / grain.release_time_samples);
            }
            break;
        case (ENVELOPE_MODE::LOGARITHMIC_CURVE):
            if (grain.grain_counter_frames < grain.attack_to_sustain_boundary_sample_idx ||
                grain.grain_counter_frames > grain.sustain_to_decay_boundary_sample_idx) {
                grain.exp_now *= grain.exp_mul;
                grain.amp = (1 - (grain.exp_now - grain.exp_min));
            } else if (grain.grain_counter_frames == grain.attack_to_sustain_boundary_sample_idx) {
                grain.amp = 1.;
            } else if (grain.grain_counter_frames == grain.sustain_to_decay_boundary_sample_idx) {
                grain.exp_now = grain.exp_min;
                grain.exp_mul =
                        pow((grain.exp_min + 1) / grain.exp_min, 1 / grain.release_time_samples);
            }
            break;

    }

    if (log_env_val && i == 0) {
        if (grain.amp < 0.0 || grain.amp > 1.0)
            __android_log_print(ANDROID_LOG_ERROR, "ENV",
                                "val OUTTA RANGE MOFO:%f !! START_IDX:%d END_IDX:%d POS:%f LEN:%d FRAMECOUNT:%d(usecount:%d)",
                                grain.amp, grain.audiobuffer_starting_idx,
                                grain.audiobuffer_starting_idx + grain.grain_len_frames,
                                grain.audiobuffer_cur_pos, grain.grain_len_frames,
                                grain.grain_counter_frames,
                                grain.use_count);
    }

//    if (grain.active && (grain.amp > 1.0 || grain.amp < 0.0)) {
//        __android_log_print(ANDROID_LOG_ERROR, "VOL", "Volume OUT OF BOUNDS!!!:%f", grain.amp);
//    }


    return out;
}


void SamplePlayer::SetParam(std::string val_name, double val) {

    if (val_name.compare("grains_per_second") == 0) {
        SetGrainsPerSecond(val);
    } else if (val_name.compare("grain_duration") == 0) {
        SetGrainDuration(val);
    } else if (val_name.compare("grain_fudge") == 0) {
        SetGrainFudge(val);
    } else if (val_name.compare("grain_spray") == 0) {
        SetGrainSpray(val);
    } else if (val_name.compare("grain_index") == 0) {
        SetGrainIndex(val);
    } else if (val_name.compare("granular_mode") == 0) {
        SetGranularMode(val);
    } else if (val_name.compare("envelope_mode") == 0) {
        SetEnvelopeMode(val);
    } else if (val_name.compare("scramble_mode") == 0) {
        SetScrambleMode();
    } else if (val_name.compare("stutter_mode") == 0) {
        SetStutterMode();
    }
}

float SamplePlayer::GetParam(std::string val_name) {
    if (val_name.compare("grains_per_second") == 0) {
        return grains_per_sec_;
    } else if (val_name.compare("grain_index") == 0) {
        float idx = GetGrainIndex();
        __android_log_print(ANDROID_LOG_ERROR, "GET IDX returtn", "idx:%f", idx);
        return idx;
    } else if (val_name.compare("grain_duration") == 0) {
        return grain_duration_ms_;
    } else if (val_name.compare("grain_fudge") == 0) {
        return granular_fudge_ms;
    } else if (val_name.compare("grain_spray") == 0) {
        return granular_spray_ms_;
    } else if (val_name.compare("grain_index") == 0) {
        return GetGrainIndex();
    } else if (val_name.compare("envelope_mode") == 0) {
        if (envelope_mode_ == ENVELOPE_MODE::PARABOLIC)
            return 0.;
        else if (envelope_mode_ == ENVELOPE_MODE::TRAPEZOIDAL)
            return 1.;
        else if (envelope_mode_ == ENVELOPE_MODE::EXPONENTIAL_CURVE)
            return 2.;
        else if (envelope_mode_ == ENVELOPE_MODE::LOGARITHMIC_CURVE)
            return 3.;
    } else if (val_name.compare("loop_mode") == 0) {
        if (loop_mode_ == LOOP_MODE::LOOP)
            return 1.;
        return 0;
    }

    return -1.;
}

void SamplePlayer::SetGrainsPerSecond(double val) {
    grains_per_sec_ = val;
}

void SamplePlayer::SetGrainDuration(double val) {
    grain_duration_ms_ = val;
}

void SamplePlayer::SetGrainFudge(double val) {
    granular_fudge_ms = val;
}

void SamplePlayer::SetGrainSpray(double val) {
    granular_spray_ms_ = val;
}

void SamplePlayer::SetGrainIndex(int val) {
    if (val >= 0 && val <= 100) {
        double pos = sample_data_.data_len / 100. * val;
        read_idx_ = pos;
    }
    __android_log_print(ANDROID_LOG_ERROR, "READIX",
                        "ID:%d", val);
}

int SamplePlayer::GetGrainIndex() {
    int percent = 100. / sample_data_.data_len * read_idx_;
    __android_log_print(ANDROID_LOG_ERROR, "READzzz", "idx:%d", percent);
    return percent;

}

void SamplePlayer::SetGranularMode(int val) {

    // TODO - fix - this is a toggle
    if (loop_mode_ == LOOP_MODE::LOOP)
        loop_mode_ = LOOP_MODE::SMUDGE;
    else
        loop_mode_ = LOOP_MODE::LOOP;

}

void SamplePlayer::SetEnvelopeMode(int val) {
    if (val == 0)
        envelope_mode_ = ENVELOPE_MODE::PARABOLIC;
    else if (val == 1)
        envelope_mode_ = ENVELOPE_MODE::TRAPEZOIDAL;
    else if (val == 2)
        envelope_mode_ = ENVELOPE_MODE::EXPONENTIAL_CURVE;
    else if (val == 3)
        envelope_mode_ = ENVELOPE_MODE::LOGARITHMIC_CURVE;

    __android_log_print(ANDROID_LOG_ERROR, "ENVELOPEOPOPOPE",
                        "MODE:%d", val);

}

void SamplePlayer::SetStutterMode() {
    stutter_pending_ = true;
}


void SamplePlayer::SetScrambleMode() {
    scramble_pending_ = true;
}

