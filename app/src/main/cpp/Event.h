//
// Created by Thorsten Sideboard on 2019-10-16.
//

#ifndef GRANNYNORMAN_EVENT_H
#define GRANNYNORMAN_EVENT_H



    enum class EventType {
        MIDI_TICK,
        BPM_CHANGE,
        CHORD_CHANGE,
        SEQUENCER_NOTE,
    };

    struct Event {

        Event(int midi_tick) : midi_tick{midi_tick} {}

        EventType type{EventType::MIDI_TICK};

        int midi_tick{0};
        bool is_thirtysecond{false};
        bool is_twentyfourth{false};
        bool is_sixteenth{false};
        bool is_twelth{false};
        bool is_eighth{false};
        bool is_sixth{false};
        bool is_quarter{false};
        bool is_third{false};
        bool is_start_of_bar{false};

        //        double time_of_next_midi_tick;
        //        int loop_beat;
        //        bool loop_started;
        //        unsigned int size_of_thirtysecond_note;
        //        unsigned int size_of_sixteenth_note;
        //        unsigned int size_of_eighth_note;
        //        unsigned int size_of_quarter_note;
    };


#endif // GRANNYNORMAN_EVENT_H
