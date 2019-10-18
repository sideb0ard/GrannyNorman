
//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_SOUNDGENERATOR_H
#define GRANNYNORMAN_SOUNDGENERATOR_H

#include "DefJams.h"
#include "Event.h"
#include "TimingData.h"





    class SoundGenerator {

    public:
        virtual StereoValue Generate(TimingData timing_data) = 0;

        virtual void EventNotify(Event ev) = 0;

        virtual ~SoundGenerator() = default;

    };




#endif //GRANNYNORMAN_SOUNDGENERATOR_H
