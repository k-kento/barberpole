#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vulkan/vulkan_android.h>
#include "surface.h"
#include "log.h"

bool Surface::create(VulkanContext *vkContext, ANativeWindow *window) {
    // 古い surface と window を解放
    if (mSurface) {
        vkDestroySurfaceKHR(vkContext->getVulkan()->getVkInstance(), mSurface, nullptr);
    }
    if (mWindow) {
        ANativeWindow_release(mWindow);
    }

    mWindow = window;
    ANativeWindow_acquire(mWindow);

    VkAndroidSurfaceCreateInfoKHR surfInfo{};
    surfInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfInfo.window = window;

    auto result = vkCreateAndroidSurfaceKHR(vkContext->getVulkan()->getVkInstance(), &surfInfo, nullptr, &mSurface);
    if (result != VK_SUCCESS) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
        LOGE("Failed to init surface: %d", result);
        return false;
    }
    return true;
}

void Surface::destroy(VulkanContext *vkContext) {
    if (mSurface) {
        vkDestroySurfaceKHR(vkContext->getVulkan()->getVkInstance(), mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
    }
    if (mWindow) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }
}
