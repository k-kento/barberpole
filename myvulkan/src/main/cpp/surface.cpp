#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vulkan/vulkan_android.h>
#include "surface.h"
#include "log.h"

bool Surface::create(VulkanContext *vkContext, ANativeWindow *window) {
    mWindow = window;
    VkAndroidSurfaceCreateInfoKHR surfInfo{};
    surfInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfInfo.window = window;
    if (vkCreateAndroidSurfaceKHR(vkContext->getVkInstance(), &surfInfo, nullptr, &mSurface) != VK_SUCCESS) {
        LOGE("Failed to create surface");
        return false;
    }
    return true;
}

void Surface::destroy(VulkanContext *vkContext) {
    if (mSurface) vkDestroySurfaceKHR(vkContext->getVkInstance(), mSurface, nullptr);
    if (mWindow) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }
}

VkSurfaceKHR Surface::getSurface() {
    return mSurface;
}