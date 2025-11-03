#include <android/native_window_jni.h>
#include <jni.h>
#include "drawing_renderer.hpp"
#include "surface.h"
#include "swap_chain.hpp"
#include "vulkan_utils.h"
#include "touch_message.hpp"
#include "suraface_changed_message.hpp"
#include "drawing_engine.hpp"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeInit(JNIEnv *env,
                                                                  jobject thiz,
                                                                  jobject androidSurface,
                                                                  jlong vulkanContextHandle) {
    auto *vkContext = reinterpret_cast<VulkanContext *>(vulkanContextHandle);
    ANativeWindow *window = ANativeWindow_fromSurface(env, androidSurface);

    auto surface = std::make_unique<Surface>(vkContext->getVkInstance(), window);
    auto engine = new DrawingEngine(*vkContext, std::move(surface));

    return reinterpret_cast<jlong>(engine);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeOnSurfaceChanged(JNIEnv *env,
                                                                              jobject,
                                                                              jlong nativeHandle,
                                                                              jobject androidSurface,
                                                                              jint deviceRotationDegree) {
    auto *engine = reinterpret_cast<DrawingEngine *>(nativeHandle);
    if (engine) {
        ANativeWindow *window = ANativeWindow_fromSurface(env, androidSurface); // TODO

        auto windowWidth = ANativeWindow_getWidth(window);
        auto windowHeight = ANativeWindow_getHeight(window);

        auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
        LOGD("Window: %d,%d", windowWidth, windowHeight);
        auto projectionMatrix = VulkanUtils::generateProjectionMatrix(deviceRotationDegree, viewBounds);
        auto message = std::make_unique<SurfaceChangedMessage>(viewBounds, projectionMatrix);
        engine->postMessage(std::move(message));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeStart(JNIEnv *env, jobject, jlong nativeHandle) {
    auto *engine = reinterpret_cast<DrawingEngine *>(nativeHandle);
    if (engine) engine->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeStop(JNIEnv *env,
                                                                  jobject thiz,
                                                                  jlong nativeHandle) {
    auto *engine = reinterpret_cast<DrawingEngine *>(nativeHandle);
    if (engine) engine->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeDestroy(JNIEnv *env,
                                                                     jobject thiz,
                                                                     jlong nativeHandle) {
    auto *engine = reinterpret_cast<DrawingEngine *>(nativeHandle);
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
    auto *engine = reinterpret_cast<DrawingEngine *>(nativeHandle);
    auto message = std::make_unique<TouchMessage>(TouchMessage::TouchType::Move, x, y);
    engine->postMessage(std::move(message));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_drawing_DrawingRenderer_nativeNotifyTouchUpEvent(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jlong nativeHandle) {
    auto *engine = reinterpret_cast<DrawingEngine *>(nativeHandle);
    auto message = std::make_unique<TouchMessage>(TouchMessage::TouchType::UP);
    engine->postMessage(std::move(message));
}
