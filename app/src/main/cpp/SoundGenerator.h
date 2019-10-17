
//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_SOUNDGENERATOR_H
#define GRANNYNORMAN_SOUNDGENERATOR_H

#include "Event.h"

namespace grannynorman {

class SoundGenerator {

public:
    virtual double Generate() = 0;

    virtual void EventNotify(Event ev) = 0;

};


} // namespace

#endif //GRANNYNORMAN_SOUNDGENERATOR_H
