//
// Created by Thorsten Sideboard on 2019-10-16.
//

#ifndef GRANNYNORMAN_EVENT_H
#define GRANNYNORMAN_EVENT_H

namespace grannynorman {


    enum class EventType{
            MIDI_TICK,
            BPM_CHANGE,
            CHORD_CHANGE,
            SEQUENCER_NOTE,
    };

    struct Event {

        Event(EventType type, int midi_tick) : type{type}, midi_tick{midi_tick} {};

        EventType type;
        int midi_tick;

        bool is_thirtysecond{false};
        bool is_twentyfourth{false};
        bool is_sixteenth{false};
        bool is_twelth{false};
        bool is_eighth{false};
        bool is_sixth{false};
        bool is_quarter{false};
        bool is_third{false};
        bool is_start_of_bar{false};
    };

}


#endif //GRANNYNORMAN_EVENT_H
