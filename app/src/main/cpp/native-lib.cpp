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

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_toggleGranularMode(JNIEnv *env, jobject instance)
{
    engine.SetGranularMode(1);
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainsPerSecond(JNIEnv *env, jobject instance)
{
    return engine.GetGrainsPerSecond();
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainDuration(JNIEnv *env, jobject instance)
{
    return engine.GetGrainDuration();
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainSpray(JNIEnv *env, jobject instance)
{
    return engine.GetGrainSpray();
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainFudge(JNIEnv *env, jobject instance)
{
    return engine.GetGrainFudge();
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainIndex(JNIEnv *env, jobject instance)
{
    return engine.GetGrainIndex();
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_toggleOnOffMode(JNIEnv *env, jobject instance)
{
    engine.ToggleOnOff();
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setEnvelopeMode(JNIEnv *env, jobject instance, jint mode)
{
    engine.SetEnvelopeMode(mode);
}


JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_reset(JNIEnv *env, jobject instance) {
    engine.Reset();
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_scramble(JNIEnv *env, jobject instance) {
    engine.Scramble();
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_stutter(JNIEnv *env, jobject instance) {
    engine.Stutter();
}

} // extern

