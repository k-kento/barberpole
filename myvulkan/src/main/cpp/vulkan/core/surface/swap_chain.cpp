#include "swap_chain.h"

SwapChain::SwapChain(VulkanContext *vkContext, vk::SurfaceKHR surface) {
    auto physicalDevice = vkContext->getPhysicalDevice();
    auto device = vkContext->getDevice();

    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

    auto formats = physicalDevice.getSurfaceFormatsKHR(surface);
    if (formats.empty()) {
        throw std::runtime_error("No surface formats available");
    }

    // 最適なフォーマットを選択
    vk::SurfaceFormatKHR surfaceFormat = formats[0];
    for (const auto &f: formats) {
        if (f.format == vk::Format::eB8G8R8A8Unorm &&
            f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            surfaceFormat = f;
            break;
        }
    }

    mFormat = surfaceFormat.format;

    // 解像度
    vk::Extent2D extent;
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
        extent = surfaceCapabilities.currentExtent;
    } else {
        // TODO: 画面サイズを取得
        throw std::runtime_error("No surface extent available.");
    }
    mExtent = extent;

    uint32_t imageCount = std::min(surfaceCapabilities.minImageCount + 1,
                                   surfaceCapabilities.maxImageCount > 0 ? surfaceCapabilities.maxImageCount
                                                                         : UINT32_MAX);

    vk::SwapchainCreateInfoKHR scInfo{};
    scInfo.surface = surface;
    scInfo.minImageCount = imageCount;
    scInfo.imageFormat = mFormat;
    scInfo.imageColorSpace = surfaceFormat.colorSpace;
    scInfo.imageExtent = extent;
    scInfo.imageArrayLayers = 1;
    scInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    scInfo.imageSharingMode = vk::SharingMode::eExclusive;
    scInfo.preTransform = surfaceCapabilities.currentTransform;
    scInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    scInfo.presentMode = selectPresentMode(physicalDevice, surface);
    scInfo.clipped = VK_TRUE;

    mSwapChain = device.createSwapchainKHRUnique(scInfo);

    mImages = device.getSwapchainImagesKHR(mSwapChain.get());
    if (mImages.empty()) {
        LOGE("No swap chain images retrieved.");
    }

    try {
        mImageViews = createImageViews(device, mImages, mFormat);
        if (mImageViews.empty()) {
            LOGE("images views size == 0");
        }
    } catch (const vk::SystemError &e) {
        throw std::runtime_error(std::string("Failed to create image views: ") + e.what());
    }
}

std::vector<vk::UniqueImageView>
SwapChain::createImageViews(vk::Device device, const std::vector<vk::Image> &images, vk::Format format) {
    std::vector<vk::UniqueImageView> imageViews;

    imageViews.reserve(images.size());

    for (auto image: images) {
        vk::ImageViewCreateInfo ivInfo{};
        ivInfo.image = image;
        ivInfo.viewType = vk::ImageViewType::e2D;
        ivInfo.format = format;
        ivInfo.components = {
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity
        };
        ivInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        ivInfo.subresourceRange.baseMipLevel = 0;
        ivInfo.subresourceRange.levelCount = 1;
        ivInfo.subresourceRange.baseArrayLayer = 0;
        ivInfo.subresourceRange.layerCount = 1;

        imageViews.push_back(device.createImageViewUnique(ivInfo));
    }

    return imageViews;
}

vk::PresentModeKHR SwapChain::selectPresentMode(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
    auto modes = physicalDevice.getSurfacePresentModesKHR(surface);

    for (auto mode: modes) {
        if (mode == vk::PresentModeKHR::eMailbox)
            return mode;
    }

    return vk::PresentModeKHR::eFifo;
}
