#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>

#include "AudioEngine.h"


AudioEngine engine;


extern "C" {

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_startEngine(JNIEnv *env, jobject instance,
                                                            jobject assetManager) {
    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    engine.Start(mgr);
}



JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainsPerSecond(JNIEnv *env, jobject instance, jfloat gps)
{
    engine.SetGrainsPerSecond(gps);
}


JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainDuration(JNIEnv *env, jobject instance, jfloat dur)
{
    engine.SetGrainDuration(dur);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainSpray(JNIEnv *env, jobject instance, jfloat val)
{
    engine.SetGrainSpray(val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainFudge(JNIEnv *env, jobject instance, jfloat val)
{
    engine.SetGrainFudge(val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGranularMode(JNIEnv *env, jobject instance, jint val)
{
    engine.SetGranularMode(val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainIndex(JNIEnv *env, jobject instance, jint val)
{
    engine.SetGrainIndex(val);
}

} // extern

