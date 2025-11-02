#include <android/native_window_jni.h>
#include <jni.h>
#include "drawing_renderer.hpp"
#include "surface.h"
#include "swap_chain.h"
#include "vulkan_engine.hpp"
#include "vulkan_utils.h"
#include "touch_message.hpp"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeInit(JNIEnv *env,
                                                                       jobject thiz,
                                                                       jobject androidSurface,
                                                                       jlong vulkanContextHandle,
                                                                       jint deviceRotationDegree) {
    auto *vkContext = reinterpret_cast<VulkanContext *>(vulkanContextHandle);
    ANativeWindow *window = ANativeWindow_fromSurface(env, androidSurface);

    auto windowWidth = ANativeWindow_getWidth(window);
    auto windowHeight = ANativeWindow_getHeight(window);

    auto surface = std::make_unique<Surface>(vkContext->getVkInstance(), window);
    auto swapChain = std::make_unique<SwapChain>(vkContext, surface->getSurface());
    auto renderPass = std::make_unique<RenderPass>(vkContext->getDevice(), swapChain->getFormat());

    auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
    auto projectionMatrix = VulkanUtils::generateProjectionMatrix(deviceRotationDegree, viewBounds);

    auto renderer = std::make_unique<DrawingRenderer>(*vkContext,
                                                      *renderPass,
                                                      viewBounds,
                                                      projectionMatrix);

    auto *engine = new VulkanEngine(*vkContext,
                                    std::move(surface),
                                    std::move(swapChain),
                                    std::move(renderPass),
                                    std::move(renderer));
    return reinterpret_cast<jlong>(engine);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeStart(JNIEnv *env, jobject, jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) engine->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeStop(JNIEnv *env,
                                                                       jobject thiz,
                                                                       jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) engine->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeDestroy(JNIEnv *env,
                                                                          jobject thiz,
                                                                          jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    if (engine) {
        delete engine;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeNotifyTouchMoveEvent(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jlong nativeHandle,
                                                                                   jfloat x,
                                                                                   jfloat y) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    auto message = std::make_unique<TouchMessage>(TouchMessage::TouchType::Move, x, y);
    engine->postMessage(std::move(message));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeNotifyTouchUpEvent(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jlong nativeHandle) {
    auto *engine = reinterpret_cast<VulkanEngine *>(nativeHandle);
    auto message = std::make_unique<TouchMessage>(TouchMessage::TouchType::UP);
    engine->postMessage(std::move(message));
}

