#pragma once

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include "vulkan_context.h"

class Surface {
public:
    Surface() = default;

    ~Surface() = default;

    bool create(VulkanContext *vkContext, ANativeWindow *window);

    void destroy(VulkanContext *vkContext);

    [[nodiscard]] VkSurfaceKHR getSurface() const {
        return mSurface;
    }

private:
    VkSurfaceKHR mSurface = VK_NULL_HANDLE;
    ANativeWindow *mWindow = nullptr;
};
