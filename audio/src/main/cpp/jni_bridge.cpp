#include <jni.h>
#include "wav_loader.h"
#include "audio_engine.h"
#include "log_util.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

// TODO メモリーリーク
AudioEngine* audioEngine = nullptr;

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioEngine_load(JNIEnv* env, jobject, jobject assetManager) {
    AAssetManager* gAssetManager = AAssetManager_fromJava(env, assetManager);
    if (!gAssetManager) return;

    WavLoader loader(gAssetManager);
    if (loader.load("trumpet.wav")) {
        std::vector<int16_t> pcm = loader.getPcmData();
        uint16_t ch = loader.getChannels();
        uint32_t sr = loader.getSampleRate();

        audioEngine = new AudioEngine();

        if (!(audioEngine->load(sr, ch, pcm))) {
            LOGE("AudioEngine failed to load.");
        }
    } else {
        LOGE("failed to load.");
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioEngine_startAudio(JNIEnv *env, jobject /* this */) {
    if (audioEngine) {
        audioEngine->start();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_soundlib_AudioEngine_stopAudio(JNIEnv *env, jobject /* this */) {
    if (audioEngine) {
        audioEngine->stop();
    }
}
