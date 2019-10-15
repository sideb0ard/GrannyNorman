//
// Created by Thorsten Sideboard on 2019-10-14.
//

#ifndef GRANNYNORMAN_LINKMANAGER_H
#define GRANNYNORMAN_LINKMANAGER_H

#include <ableton/Link.hpp>

class LinkManager {
public:
    LinkManager();
    double getTempo();
    double getPhase();
    void enable();
private:
    ableton::Link link;
};


#endif //GRANNYNORMAN_LINKMANAGER_H
