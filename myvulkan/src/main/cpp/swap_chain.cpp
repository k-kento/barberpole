#include "swap_chain.h"
#include "log.h"

//
// Created by 黒須権人 on 2025/09/20.
//
bool SwapChain::create(VulkanContext *vkContext, VkSurfaceKHR surface) {
    VkPhysicalDevice physicalDevice = vkContext->getPhysicalDevice();
    VkDevice device = vkContext->getDevice();

    VkSurfaceCapabilitiesKHR caps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &caps);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
    VkSurfaceFormatKHR surfaceFormat = formats[0];
    mFormat = surfaceFormat.format;

    VkExtent2D extent = caps.currentExtent;
    if (extent.width == UINT32_MAX) {
        extent.width = 640;
        extent.height = 480;
    }
    mExtent = extent;

    VkSwapchainCreateInfoKHR scInfo{};
    scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    scInfo.surface = surface;
    scInfo.minImageCount = caps.minImageCount + 1;
    if (scInfo.minImageCount > caps.maxImageCount && caps.maxImageCount > 0)
        scInfo.minImageCount = caps.maxImageCount;
    scInfo.imageFormat = mFormat;
    scInfo.imageColorSpace = surfaceFormat.colorSpace;
    scInfo.imageExtent = extent;
    scInfo.imageArrayLayers = 1;
    scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    scInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    scInfo.preTransform = caps.currentTransform;
    scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    scInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    scInfo.clipped = VK_TRUE;
    scInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &scInfo, nullptr, &mSwapChain) != VK_SUCCESS) {
        LOGE("Failed to create swapchain");
        return false;
    }

    uint32_t imgCount = 0;
    vkGetSwapchainImagesKHR(device, mSwapChain, &imgCount, nullptr);
    mImages.resize(imgCount);
    vkGetSwapchainImagesKHR(device, mSwapChain, &imgCount, mImages.data());

    if (!createImageViews(vkContext)) {
        return false;
    }

    return true;
}

void SwapChain::destroy(VkDevice device) {
    if (mSwapChain) vkDestroySwapchainKHR(device, mSwapChain, nullptr);
}

bool SwapChain::createImageViews(VulkanContext *vkContext) {
    mImageViews.resize(mImages.size());
    for (size_t i = 0; i < mImages.size(); ++i) {
        VkImageViewCreateInfo ivInfo{};
        ivInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ivInfo.image = mImages[i];
        ivInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ivInfo.format = mFormat;
        ivInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ivInfo.subresourceRange.baseMipLevel = 0;
        ivInfo.subresourceRange.levelCount = 1;
        ivInfo.subresourceRange.baseArrayLayer = 0;
        ivInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(vkContext->getDevice(), &ivInfo, nullptr, &mImageViews[i]) != VK_SUCCESS) {
            LOGE("Failed to create image view %zu", i);
            return false;
        }
    }
    return true;
}

VkSwapchainKHR SwapChain::getSwapChain() const {
    return mSwapChain;
}

const std::vector<VkImageView> &SwapChain::getImageViews() {
    return mImageViews;
}

VkFormat SwapChain::getFormat() {
    return mFormat;
}

VkExtent2D SwapChain::getExtent() {
    return mExtent;
}