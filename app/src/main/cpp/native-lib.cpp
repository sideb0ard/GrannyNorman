#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>

#include "AudioEngine.h"


AudioEngine engine;


extern "C" {

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_startEngine(JNIEnv *env, jobject instance) {
    engine.start();
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_tap(JNIEnv *env, jobject instance, jboolean b) {
    engine.tap(b);
}


JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setFrequency(JNIEnv *env, jobject instance,
                                                             jfloat frequency) {
    engine.setFrequency(frequency);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_loadSamples(JNIEnv *env, jobject instance,
                                                            jobject assetManager) {

    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    engine.LoadSamples(mgr);
}

}
