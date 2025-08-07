#include <jni.h>
#include "wav_loader.h"
#include "audio_engine.h"
#include "log_util.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <algorithm> // for std::transform
#include <string>      // for std::string

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_soundlib_AudioLib_createNativeEngine(JNIEnv *env, jobject) {
    auto *engine = new AudioEngine();
    return reinterpret_cast<jlong>(engine);
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_load(JNIEnv *env, jobject, jlong nativeHandle, jobject assetManager, jstring fileName) {
    AudioEngine *engine = reinterpret_cast<AudioEngine *>(nativeHandle);
    if (!engine) {
        LOGE("Engine is null");
        return;
    }

    AAssetManager *gAssetManager = AAssetManager_fromJava(env, assetManager);
    if (!gAssetManager) {
        LOGE("AssetManager is null");
        return;
    }

    const char* fileNameCStr = env->GetStringUTFChars(fileName, nullptr);
    if (!fileNameCStr) {
        LOGE("Failed to get UTF chars from jstring");
        return;
    }
    std::string fileNameStr(fileNameCStr);
    env->ReleaseStringUTFChars(fileName, fileNameCStr);

    WavLoader loader(gAssetManager);
    if (loader.load(fileNameCStr)) {
        LOGD("Successfully loaded WAV file: %s", fileNameCStr);
        std::vector<int16_t> pcm = loader.getPcmData();
        uint16_t ch = loader.getChannels();
        uint32_t sr = loader.getSampleRate();
        if (!(engine->load(sr, ch, pcm))) {
            LOGE("AudioEngine failed to load PCM data for: %s", fileNameCStr);
        } else {
            LOGD("AudioEngine successfully loaded PCM data for: %s", fileNameCStr);
        }
    } else {
        LOGE("WavLoader failed to load: %s", fileNameCStr);
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_start(JNIEnv *env, jobject, jlong nativeHandle) {
    AudioEngine *engine = reinterpret_cast<AudioEngine *>(nativeHandle);
    if (engine) {
        engine->start();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_stop(JNIEnv *env, jobject, jlong nativeHandle) {
    AudioEngine *engine = reinterpret_cast<AudioEngine *>(nativeHandle);
    if (engine) {
        engine->stop();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_release(JNIEnv *env, jobject, jlong nativeHandle) {
    AudioEngine *engine = reinterpret_cast<AudioEngine *>(nativeHandle);
    delete engine;
}