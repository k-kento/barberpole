#pragma once

#include <vector>

#include "log.h"
#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

class SwapChain {
   public:
    SwapChain(VulkanContext& vkContext, vk::SurfaceKHR surface);

    uint32_t acquireNextImage();

    void present();

    [[nodiscard]] vk::Format getFormat() const { return mFormat; }

    [[nodiscard]] vk::Extent2D getExtent() const { return mExtent; }

    [[nodiscard]] const std::vector<vk::Image>& getImages() const { return mImages; }

    [[nodiscard]] const uint32_t getCurrentImageIndex() const { return mCurrentImageIndex; }

    [[nodiscard]] std::vector<vk::ImageView> getImageViews() const {
        std::vector<vk::ImageView> result;
        result.reserve(mImageViews.size());
        for (const auto& u : mImageViews) {
            result.push_back(u.get());
        }
        return result;
    }

    [[nodiscard]] vk::Semaphore getImageAvailable() const { return mImageAvailable[mCurrentImageIndex].get(); }

    [[nodiscard]] vk::Semaphore getRenderFinished() const { return mRenderFinished[mCurrentImageIndex].get(); }

   private:
    VulkanContext& mVkContext;
    vk::UniqueSwapchainKHR mSwapChain;
    std::vector<vk::Image> mImages;
    uint32_t mCurrentImageIndex = 0;
    std::vector<vk::UniqueImageView> mImageViews;
    vk::Format mFormat;
    vk::Extent2D mExtent;

    std::queue<vk::UniqueSemaphore> mImageAvailablePool;
    std::vector<vk::UniqueSemaphore> mImageAvailable;
    std::vector<vk::UniqueSemaphore> mRenderFinished;

    static std::vector<vk::UniqueImageView> createImageViews(vk::Device device,
                                                             const std::vector<vk::Image>& images,
                                                             vk::Format format);

    static vk::PresentModeKHR selectPresentMode(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};
