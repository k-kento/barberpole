#include "surface.h"

#include <android/native_window.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_android.h>

#include "log.h"

Surface::Surface(vk::Instance vkInstance, ANativeWindow* window) : mWindow(window) {
    ANativeWindow_acquire(mWindow);

    vk::AndroidSurfaceCreateInfoKHR surfInfo{};
    surfInfo.window = window;

    mSurface = vkInstance.createAndroidSurfaceKHRUnique(surfInfo, nullptr);
}

Surface::~Surface() {
    ANativeWindow_release(mWindow);
    mWindow = nullptr;
    LOGI("Surface destroyed");
}
