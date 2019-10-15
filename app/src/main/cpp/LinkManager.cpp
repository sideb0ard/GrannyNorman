//
// Created by Thorsten Sideboard on 2019-10-14.
//

#include <android/log.h>

#include "LinkManager.h"

LinkManager::LinkManager() : link{120.} {
    link.enable(true);
    __android_log_print(ANDROID_LOG_ERROR, "WOOP", "LINK ENABLED BOO YA:");
}