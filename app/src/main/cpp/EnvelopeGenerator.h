//
// Created by Thorsten Sideboard on 2019-10-16.
//

#ifndef GRANNYNORMAN_ENVELOPEGENERATOR_H
#define GRANNYNORMAN_ENVELOPEGENERATOR_H



    enum class Mode {
        ANALOG,
        DIGITAL,
    };

    enum class State {
        OFF,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE,
        SHUTDOWN,
    };

    class EnvelopeGenerator {

        bool sustain_override_;
        bool release_pending_;

        Mode mode_; // enum above, analog or digital
        // special modes
        bool reset_to_zero_;
        bool legato_mode_;
        bool output_eg_; // i.e. this instance is going direct to output, rather
        // than into an intermediatery

        double output_;

        double attack_coeff_;
        double attack_offset_;
        double attack_tco_;

        double decay_coeff_;
        double decay_offset_;
        double decay_tco_;

        double release_coeff_;
        double release_offset_;
        double release_tco_;

        double attack_time_msec_;
        double decay_time_msec_;
        double release_time_msec_;

        double shutdown_time_msec_;

        double sustain_level_;

        double attack_time_scalar_; // for velocity -> attack time mod
        double decay_time_scalar_;  // for note# -> decay time mod

        double inc_shutdown_;

        // enum above
        State state_;
    };



#endif //GRANNYNORMAN_ENVELOPEGENERATOR_H
