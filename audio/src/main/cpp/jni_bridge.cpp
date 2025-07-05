#include <jni.h>
#include "wav_loader.h"
#include "audio_engine.h"
#include "log_util.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

std::unique_ptr<AudioEngine> audioEngine;

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_load(JNIEnv* env, jobject, jobject assetManager, jstring type) {
    AAssetManager* gAssetManager = AAssetManager_fromJava(env, assetManager);
    if (!gAssetManager) return;

    const char* typeStr = env->GetStringUTFChars(type, nullptr);

    std::string fileName;

    if (strcmp(typeStr, "taiko") == 0) {
        fileName = "taiko_drums.wav";
    } else {
        fileName = "taiko_drums.wav";
    }

    WavLoader loader(gAssetManager);
    if (loader.load(fileName.c_str())) {
        std::vector<int16_t> pcm = loader.getPcmData();
        uint16_t ch = loader.getChannels();
        uint32_t sr = loader.getSampleRate();

        audioEngine = std::make_unique<AudioEngine>();

        if (!(audioEngine->load(sr, ch, pcm))) {
            LOGE("AudioEngine failed to load.");
        }
    } else {
        LOGE("failed to load.");
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_start(JNIEnv *env, jobject /* this */) {
    if (audioEngine) {
        audioEngine->start();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_stop(JNIEnv *env, jobject /* this */) {
    if (audioEngine) {
        audioEngine->stop();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioLib_release(JNIEnv *env, jobject /* this */) {
    if (audioEngine) {
        audioEngine->stop();      // 念のため停止
        audioEngine.reset();      // 破棄（unique_ptrなので自動でdeleteされる）
        LOGI("AudioEngine released");
    }
}