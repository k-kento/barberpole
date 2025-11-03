#include <android/native_window_jni.h>
#include <jni.h>
#include "kaleidoscope_renderer.h"
#include "surface.h"
#include "swap_chain.h"
#include "vulkan_engine.hpp"
#include "rotation_message.hpp"
#include "update_texture_message.hpp"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeInit(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jobject androidSurface,
                                                                            jlong vulkanContextHandle,
                                                                            jstring jFilePath,
                                                                            jint deviceRotationDegree) {
    auto *vkContext = reinterpret_cast<VulkanContext *>(vulkanContextHandle);
    ANativeWindow *window = ANativeWindow_fromSurface(env, androidSurface);

    auto windowWidth = ANativeWindow_getWidth(window);
    auto windowHeight = ANativeWindow_getHeight(window);

    auto surface = std::make_unique<Surface>(vkContext->getVkInstance(), window);
    auto swapChain = std::make_unique<SwapChain>(*vkContext, surface->getSurface());
    auto renderPass = std::make_unique<RenderPass>(vkContext->getDevice(), swapChain->getFormat());

    const char *cStr = env->GetStringUTFChars(jFilePath, nullptr);
    std::string filePath(cStr);
    env->ReleaseStringUTFChars(jFilePath, cStr);

    auto renderer = std::make_unique<KaleidoscopeRenderer>(*vkContext,
                                                           *renderPass,
                                                           windowWidth,
                                                           windowHeight,
                                                           deviceRotationDegree,
                                                           filePath);

    auto *engine = new VulkanEngine(*vkContext,
                                    std::move(surface),
                                    std::move(swapChain),
                                    std::move(renderPass),
                                    std::move(renderer));
    return reinterpret_cast<jlong>(engine);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeStart(JNIEnv *env, jobject, jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) engine->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeStop(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) engine->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeDestroy(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) {
        delete engine;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeSetRotationState(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jlong nativeHandle,
                                                                                        jint rotationState) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);

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

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeSetImage(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jlong nativeHandle,
                                                                                jstring jFilePath) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);

    const char *cStr = env->GetStringUTFChars(jFilePath, nullptr);
    std::string filePath(cStr);
    env->ReleaseStringUTFChars(jFilePath, cStr);

    auto message = std::make_unique<UpdateTextureMessage>(filePath);
    engine->postMessage(std::move(message));
}
