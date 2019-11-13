
//
// Created by Thorsten Sideboard on 2019-10-15.
//

#ifndef GRANNYNORMAN_SOUNDGENERATOR_H
#define GRANNYNORMAN_SOUNDGENERATOR_H

#include <string>
#include "DefJams.h"
#include "Event.h"
#include "TimingData.h"


class SoundGenerator {

public:
    virtual StereoValue Generate(TimingData timing_data) = 0;

    virtual void EventNotify(Event ev) = 0;

    virtual void SetParam(std::string val_name, double val) = 0;

    virtual float GetParam(std::string val_name) = 0;

    virtual void Reset() = 0;

    virtual ~SoundGenerator() = default;

    bool IsActive() { return active_; }

    void SetActive(bool b) { active_ = b;};

    bool active_{false};
    bool started_{false};

};


#endif //GRANNYNORMAN_SOUNDGENERATOR_H
