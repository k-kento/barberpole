#pragma once

#include "vulkan/vulkan.hpp"
#include <vector>
#include "vulkan_context.h"
#include "log.h"

class SwapChain {
public:
    SwapChain(VulkanContext *vkContext, vk::SurfaceKHR surface);

    [[nodiscard]] vk::Format getFormat() const { return mFormat; }

    [[nodiscard]] vk::Extent2D getExtent() const { return mExtent; }

    [[nodiscard]] const std::vector<vk::Image> &getImages() const { return mImages; }

    [[nodiscard]] std::vector<vk::ImageView> getImageViews() const {
        std::vector<vk::ImageView> result;
        result.reserve(mImageViews.size());
        for (const auto &u : mImageViews) {
            result.push_back(u.get()); // vk::UniqueImageView → vk::ImageView
        }
        return result;
    }

    [[nodiscard]] vk::SwapchainKHR getSwapChain() const { return mSwapChain.get(); }

private:
    static std::vector<vk::UniqueImageView> createImageViews(vk::Device device, const std::vector<vk::Image> &images, vk::Format format);

    static vk::PresentModeKHR selectPresentMode(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

private:
    vk::UniqueSwapchainKHR mSwapChain;
    std::vector<vk::Image> mImages;
    std::vector<vk::UniqueImageView> mImageViews;
    vk::Format mFormat;
    vk::Extent2D mExtent;
};
