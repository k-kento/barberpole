#pragma once

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>
#include "vulkan_context.h"

class SwapChain {
public:
    SwapChain() = default;

    ~SwapChain() = default;

    bool init(VulkanContext *vkContext, VkSurfaceKHR surface);

    void destroy(VkDevice device);

    [[nodiscard]] VkSwapchainKHR getSwapChain() const {
        return mSwapChain;
    }

    std::vector<VkImageView> getImageViews() {
        return mImageViews;
    }

    VkFormat getFormat() {
        return mFormat;
    }

    VkExtent2D getExtent() {
        return mExtent;
    }

private:
    bool createImageViews(VulkanContext *vkContext);
    static VkPresentModeKHR selectPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

private:
    VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mImageViews;
    VkFormat mFormat;
    VkExtent2D mExtent;
};
