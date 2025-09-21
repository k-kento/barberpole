#include <android/native_window_jni.h> // for ANativeWindow_fromSurface
#include <jni.h>
#include "vulkan_renderer.h"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeInit(JNIEnv *env, jobject thiz, jobject surface,
                                                   jlong vulkanContextHandle) {
    VulkanRenderer *renderer = new VulkanRenderer();
    VulkanContext *vkContext = reinterpret_cast<VulkanContext *>(vulkanContextHandle);
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    renderer->init(vkContext, window);

    return reinterpret_cast<jlong>(renderer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeStart(JNIEnv *env, jobject, jlong nativeHandle) {
    VulkanRenderer *renderer = reinterpret_cast<VulkanRenderer *>(nativeHandle);
    if (renderer) renderer->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeStop(JNIEnv *env, jobject thiz, jlong nativeHandle) {
    VulkanRenderer *renderer = reinterpret_cast<VulkanRenderer *>(nativeHandle);
    if (renderer) renderer->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_Renderer_nativeDestroy(JNIEnv *env, jobject thiz, jlong nativeHandle) {
    VulkanRenderer *renderer = reinterpret_cast<VulkanRenderer *>(nativeHandle);
    if (renderer) renderer->destroy();
}
