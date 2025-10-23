#include <jni.h>
#include <android/asset_manager_jni.h>
#include "vulkan_context.h"
#include "log.h"

extern "C" JNIEXPORT jlong JNICALL
Java_com_gastornisapp_myvulkan_VulkanContext_nativeCreate(JNIEnv *env, jobject, jobject assetManager) {
    try {
        AAssetManager *gAssetManager = AAssetManager_fromJava(env, assetManager);
        if (!gAssetManager) {
            LOGE("AssetManager is null");
            return 0;
        }
        auto *context = new VulkanContext(gAssetManager);
        return reinterpret_cast<jlong>(context);
    } catch (const std::exception &e) {
        LOGE("Exception: %s", e.what());
        return 0;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_gastornisapp_myvulkan_VulkanContext_nativeDestroy(JNIEnv *env, jobject, jlong nativeHandle) {
    if (nativeHandle == 0) return;
    auto *engine = reinterpret_cast<VulkanContext *>(nativeHandle);
    delete engine;
}
