#include <android/native_window_jni.h>
#include <jni.h>
#include "kaleidoscope_renderer.h"
#include "surface.h"
#include "swap_chain.h"
#include "vulkan_engine.hpp"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeInit(JNIEnv *env, jobject thiz,
                                                                            jobject androidSurface,
                                                                            jlong vulkanContextHandle) {
    auto *vkContext = reinterpret_cast<VulkanContext *>(vulkanContextHandle);
    ANativeWindow *window = ANativeWindow_fromSurface(env, androidSurface);

    auto windowWidth = ANativeWindow_getWidth(window);
    auto windowHeight = ANativeWindow_getHeight(window);

    auto surface = std::make_unique<Surface>(vkContext->getVkInstance(), window);
    auto swapChain = std::make_unique<SwapChain>(vkContext, surface->getSurface());
    auto renderPass = std::make_unique<RenderPass>(vkContext->getVkDevice(), swapChain->getFormat());

    auto renderer = std::make_unique<KaleidoscopeRenderer>(*vkContext, *renderPass, windowWidth, windowHeight);
    auto *engine = new VulkanEngine(*vkContext, std::move(surface), std::move(swapChain), std::move(renderPass),
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
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeStop(JNIEnv *env, jobject thiz,
                                                                            jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) engine->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeDestroy(JNIEnv *env, jobject thiz,
                                                                               jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) {
        delete engine;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_kaleidoscope_KaleidoscopeRenderer_nativeSetRotationState(JNIEnv *env, jobject thiz,
                                                                                        jlong nativeHandle,
                                                                                        jint rotationState) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    auto& renderer = engine->getRenderer();
    auto* kaleidoscopeRenderer = dynamic_cast<KaleidoscopeRenderer*>(&renderer);
    if (kaleidoscopeRenderer != nullptr) {
        switch (rotationState) {
            case 0:
                kaleidoscopeRenderer->setRotationState(KaleidoscopeRenderer::RotationState::None);
                break;
            case 1:
                kaleidoscopeRenderer->setRotationState(KaleidoscopeRenderer::RotationState::RotatingCW);
                break;
            case 2:
                kaleidoscopeRenderer->setRotationState(KaleidoscopeRenderer::RotationState::RotatingCCW);
                break;
            default:
                break;
        }
    }
}
