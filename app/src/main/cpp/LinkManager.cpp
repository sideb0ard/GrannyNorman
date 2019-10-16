//
// Created by Thorsten Sideboard on 2019-10-14.
//

#include <android/log.h>

#include "LinkManager.h"

LinkManager::LinkManager() : link_{120.},
                             session_state_{link_.captureAudioSessionState()},
                             micros_per_sample{1e6 / DEFAULT_SAMPLE_RATE} {
    link_.enable(true);
    __android_log_print(ANDROID_LOG_ERROR, "WOOP", "LINK ENABLED BOO YA:");
}

void LinkManager::UpdateFromAudioCallback(int num_frames) {

    const auto host_time =
            host_time_filter_.sampleTimeToHostTime(num_frames);

    buffer_begin_at_output_ =
            host_time + output_latency_;

    frame_counter_ += num_frames;


    session_state_ = link_.captureAudioSessionState();
    if (reset_beat_time_) {
        session_state_.requestBeatAtTime(0, host_time, quantum_);
        reset_beat_time_ = false;
    }

    if (requested_tempo_ > 0) {
        session_state_.setTempo(requested_tempo_, host_time);
        requested_tempo_ = 0;
    }

    link_.commitAudioSessionState(session_state_);

}

void LinkManager::UpdateMicrosPerSample(int sample_rate) {
    micros_per_sample = 1e6 / sample_rate;
}

void LinkManager::IncMidi(double beat_at_time) {

    time_of_next_midi_tick_ =
            (double) ((int) beat_at_time +
                      (midi_tick_ % PPQN) * MIDI_TICK_FRAC_OF_BEAT);

    midi_tick_++;
    is_midi_tick_ = true;
    //__android_log_print(ANDROID_LOG_ERROR, "WOOP", "MIDI TICK:%d", midi_tick_);

}

bool LinkManager::IsMidiTick(int frame_num) {

    is_midi_tick_ = false;
    const auto hostTime =
            buffer_begin_at_output_ +
            std::chrono::microseconds(llround(frame_num * micros_per_sample));

    auto beat_time = session_state_.beatAtTime(hostTime, quantum_);
    if (beat_time >= 0.) {
        if (beat_time > time_of_next_midi_tick_) {
            IncMidi(beat_time);
            return true;
        }
    }

    return false;
}