#include <android/native_window_jni.h>
#include <jni.h>

#include "engine.hpp"
#include "kaleidoscope_engine.hpp"
#include "rotation_message.hpp"
#include "surface.h"
#include "swap_chain.hpp"
#include "update_texture_message.hpp"
#include "view_bounds.hpp"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeInit(JNIEnv* env,
                                                                            jobject thiz,
                                                                            jobject androidSurface,
                                                                            jlong vulkanContextHandle,
                                                                            jstring jFilePath,
                                                                            jint deviceRotationDegree) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, androidSurface);

    auto* vkContext = reinterpret_cast<VulkanContext*>(vulkanContextHandle);
    auto surface = std::make_unique<Surface>(vkContext->getVkInstance(), window);

    const char* cStr = env->GetStringUTFChars(jFilePath, nullptr);
    std::string filePath(cStr);
    env->ReleaseStringUTFChars(jFilePath, cStr);

    return reinterpret_cast<jlong>(
        new KaleidoscopeEngine(*vkContext, std::move(surface), deviceRotationDegree, filePath));
}

extern "C" JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeStart(JNIEnv* env, jobject, jlong nativeHandle) {
    auto* engine = reinterpret_cast<Engine*>(nativeHandle);
    if (engine) engine->start();
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeStop(
    JNIEnv* env, jobject thiz, jlong nativeHandle) {
    auto* engine = reinterpret_cast<Engine*>(nativeHandle);
    if (engine) engine->stop();
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeDestroy(
    JNIEnv* env, jobject thiz, jlong nativeHandle) {
    auto* engine = reinterpret_cast<Engine*>(nativeHandle);
    if (engine) {
        delete engine;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeSetRotationState(JNIEnv* env,
                                                                                        jobject thiz,
                                                                                        jlong nativeHandle,
                                                                                        jint rotationState) {
    auto* engine = reinterpret_cast<Engine*>(nativeHandle);

    RotationState state = RotationState::None;
    switch (rotationState) {
        case 1:
            state = RotationState::RotatingCW;
            break;
        case 2:
            state = RotationState::RotatingCCW;
            break;
    }

    auto message = std::make_unique<RotationMessage>(state);
    engine->postMessage(std::move(message));
}

extern "C" JNIEXPORT void JNICALL Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeSetImage(
    JNIEnv* env, jobject thiz, jlong nativeHandle, jstring jFilePath) {
    auto* engine = reinterpret_cast<Engine*>(nativeHandle);

    const char* cStr = env->GetStringUTFChars(jFilePath, nullptr);
    std::string filePath(cStr);
    env->ReleaseStringUTFChars(jFilePath, cStr);

    auto message = std::make_unique<UpdateTextureMessage>(filePath);
    engine->postMessage(std::move(message));
}
