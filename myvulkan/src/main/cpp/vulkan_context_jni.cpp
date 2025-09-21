#include <jni.h>
#include "vulkan_context.h"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_VulkanContext_createNative(JNIEnv *env, jobject) {
    auto *context = new VulkanContext();
    context->init();
    return reinterpret_cast<jlong>(context);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_VulkanContext_release(JNIEnv *env, jobject, jlong nativeHandle) {
    VulkanContext *engine = reinterpret_cast<VulkanContext *>(nativeHandle);
    delete engine;
}
