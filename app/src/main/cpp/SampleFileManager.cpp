//
// Created by Thorsten Sideboard on 2019-10-13.
//

#include <unistd.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <iostream>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "SampleFileManager.h"

void SampleFileManager::LoadSamples(AAssetManager *mgr) {

    char const *think_sample = "thinkloop.wav";

    AAssetDir *assetDir = AAssetManager_openDir(mgr, "Sounds");
    const char *filename;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
        __android_log_print(ANDROID_LOG_ERROR, "DJOBBIO", "File:%s", filename);
        if (strncmp(filename, think_sample, strlen(think_sample)) == 0) {
            __android_log_print(ANDROID_LOG_ERROR, "DJOBBIO", "COMPARED WELL!");

            AAsset * think_wav = AAssetManager_open(mgr, "Sounds/thinkloop.wav", AASSET_MODE_BUFFER);
            if (think_wav) {
                __android_log_print(ANDROID_LOG_ERROR, "WOOP", "OPENED THINK: %d", AAsset_getLength(think_wav));
            }
        }
    }

    AAssetDir_close(assetDir);

}
