#include <android/native_window_jni.h>
#include <jni.h>
#include "kaleidoscope_renderer.h"
#include "surface.h"
#include "swap_chain.h"
#include "vulkan_engine.hpp"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeInit(JNIEnv *env, jobject thiz, jobject androidSurface,
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
Java_com_gastornisapp_myvulkan_Renderer_nativeStart(JNIEnv *env, jobject, jlong nativeHandle) {
    auto *renderer = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (renderer) renderer->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeStop(JNIEnv *env, jobject thiz, jlong nativeHandle) {
    auto *renderer = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (renderer) renderer->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeDestroy(JNIEnv *env, jobject thiz, jlong nativeHandle) {
    auto *renderer = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (renderer) {
        delete renderer;
    }
}
