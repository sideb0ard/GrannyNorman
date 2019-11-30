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
Java_com_theb0ardside_grannynorman_MainActivity_setGrainsPerSecond(JNIEnv *env, jobject instance, jint target, jfloat gps)
{
    engine.SetGrainsPerSecond(target, gps);
}


JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainDuration(JNIEnv *env, jobject instance, jint target, jfloat dur)
{
    engine.SetGrainDuration(target, dur);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainSpray(JNIEnv *env, jobject instance, jint target, jfloat val)
{
    engine.SetGrainSpray(target, val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainFudge(JNIEnv *env, jobject instance, jint target, jfloat val)
{
    engine.SetGrainFudge(target, val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGranularMode(JNIEnv *env, jobject instance, jint target, jint val)
{
    engine.SetGranularMode(target, val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setGrainIndex(JNIEnv *env, jobject instance, jint target, jint val)
{
    engine.SetGrainIndex(target, val);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_toggleGranularMode(JNIEnv *env, jobject instance, jint target)
{
    engine.SetGranularMode(target, 1);
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainsPerSecond(JNIEnv *env, jobject instance, jint target)
{
    return engine.GetGrainsPerSecond(target);
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainDuration(JNIEnv *env, jobject instance, jint target)
{
    return engine.GetGrainDuration(target);
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainSpray(JNIEnv *env, jobject instance, jint target)
{
    return engine.GetGrainSpray(target);
}

JNIEXPORT float JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainFudge(JNIEnv *env, jobject instance, jint target)
{
    return engine.GetGrainFudge(target);
}

JNIEXPORT int JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getEnvelopeMode(JNIEnv *env, jobject instance, jint target)
{
    return engine.GetEnvelopeMode(target);
}

JNIEXPORT int JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getGrainIndex(JNIEnv *env, jobject instance, jint target)
{
    return engine.GetGrainIndex(target);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_toggleOnOffMode(JNIEnv *env, jobject instance, jint target)
{
    engine.ToggleOnOff(target);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_setEnvelopeMode(JNIEnv *env, jobject instance, jint target, jint mode)
{
    engine.SetEnvelopeMode(target, mode);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_randomize(JNIEnv *env, jobject instance, jint target) {
    engine.Randomize(target);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_reset(JNIEnv *env, jobject instance, jint target) {
    engine.Reset(target);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_scramble(JNIEnv *env, jobject instance, jint target) {
    engine.Scramble(target);
}

JNIEXPORT void JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_stutter(JNIEnv *env, jobject instance, jint target) {
    engine.Stutter(target);
}

JNIEXPORT int JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getActive(JNIEnv *env, jobject instance, jint target) {
    return engine.GetActive(target);
}

JNIEXPORT int JNICALL
Java_com_theb0ardside_grannynorman_MainActivity_getLoopMode(JNIEnv *env, jobject instance, jint target) {
    return engine.GetLoopMode(target);
}


} // extern

