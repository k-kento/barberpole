#include "swap_chain.h"
#include "log.h"


bool SwapChain::init(VulkanContext *vkContext, VkSurfaceKHR surface) {
    VkPhysicalDevice physicalDevice = vkContext->getPhysicalDevice()->getPhysicalDevice();
    VkDevice device = vkContext->getDevice()->getDevice();

    VkSurfaceCapabilitiesKHR surfaceCapabilities{};
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    if (result != VK_SUCCESS) {
        LOGE("Failed to get surface surfaceCapabilities: %d", result);
        return false;
    }

    // Surface から フォーマット一覧を取得
    uint32_t formatCount = 0;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (result != VK_SUCCESS) {
        LOGE("Failed to get surface format: %d", result);
        return false;
    }

    std::vector<VkSurfaceFormatKHR> formats(formatCount);

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
    if (result != VK_SUCCESS) {
        LOGE("Failed to get surface format: %d", result);
        return false;
    }

    // 最適なフォーマットを選択
    VkSurfaceFormatKHR surfaceFormat = formats[0];
    for (const auto &f: formats) {
        if (f.format == VK_FORMAT_B8G8R8A8_UNORM && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            surfaceFormat = f;
            break;
        }
    }

    mFormat = surfaceFormat.format;

    VkExtent2D extent;
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
        extent = surfaceCapabilities.currentExtent;
    } else {
        // TODO
        extent = surfaceCapabilities.currentExtent;
    }

    mExtent = extent;

    /* VkSwapchainCreateInfoKHR 作成 */

    VkSwapchainCreateInfoKHR scInfo{};
    scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    scInfo.surface = surface;

    // 使うイメージ数（バッファ数）
    // minImageCount は GPU が要求する最小バッファ数 + 1 が安全
    scInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
    // GPU がサポートする最大バッファ数を超えないように補正
    if (scInfo.minImageCount > surfaceCapabilities.maxImageCount && surfaceCapabilities.maxImageCount > 0) {
        scInfo.minImageCount = surfaceCapabilities.maxImageCount;
    }

    // カラーフォーマットと色空間（必須）
    // Surface と一致させないと表示できない
    scInfo.imageFormat = mFormat;
    scInfo.imageColorSpace = surfaceFormat.colorSpace;

    // 描画解像度
    scInfo.imageExtent = extent;
    // 画像配列レイヤー
    scInfo.imageArrayLayers = 1;
    // 描画用途
    scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    // 複数キューで共有するかどうか
    scInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // 1つのキューでのみ使用する場合はEXCLUSIVEで高速
    scInfo.preTransform = surfaceCapabilities.currentTransform;
    // 透明度
    scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    // 見えないピクセルはクリップして表示しない
    scInfo.clipped = VK_TRUE;
    scInfo.oldSwapchain = VK_NULL_HANDLE;
    scInfo.presentMode = selectPresentMode(physicalDevice, surface);

    /* スワップチェーンを作成 */
    result = vkCreateSwapchainKHR(device, &scInfo, nullptr, &mSwapChain);
    if (result != VK_SUCCESS) {
        LOGE("Failed to init swapchain: %d", result);
        return false;
    }

    uint32_t imgCount = 0;
    result = vkGetSwapchainImagesKHR(device, mSwapChain, &imgCount, nullptr);
    if (result != VK_SUCCESS) {
        LOGE("Failed to init vkGetSwapchainImagesKHR: %d", result);
        return false;
    }

    mImages.resize(imgCount);
    result = vkGetSwapchainImagesKHR(device, mSwapChain, &imgCount, mImages.data());
    if (result != VK_SUCCESS) {
        LOGE("Failed to init vkGetSwapchainImagesKHR: %d", result);
        return false;
    }

    if (!createImageViews(vkContext)) {
        destroy(vkContext->getDevice()->getDevice());

        return false;
    }

    return true;
}

void SwapChain::destroy(VkDevice device) {
    for (auto view: mImageViews) {
        if (view) vkDestroyImageView(device, view, nullptr);
    }
    mImageViews.clear();

    if (mSwapChain) {
        vkDestroySwapchainKHR(device, mSwapChain, nullptr);
        mSwapChain = VK_NULL_HANDLE;
    }
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

        if (vkCreateImageView(vkContext->getDevice()->getDevice(), &ivInfo, nullptr, &mImageViews[i]) != VK_SUCCESS) {
            LOGE("Failed to init image view %zu", i);
            return false;
        }
    }
    return true;
}

VkPresentModeKHR SwapChain::selectPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint32_t presentModeCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (result != VK_SUCCESS || presentModeCount == 0) {
        LOGE("Failed to get present mode count: %d", result);
        return VK_PRESENT_MODE_FIFO_KHR; // デフォルト安全モード
    }

    std::vector<VkPresentModeKHR> modes(presentModeCount);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, modes.data());
    if (result != VK_SUCCESS) {
        LOGE("Failed to get present modes: %d", result);
        return VK_PRESENT_MODE_FIFO_KHR; // デフォルト安全モード
    }

    for (auto mode: modes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = mode;
            break;
        }
    }

    return presentMode;
}
