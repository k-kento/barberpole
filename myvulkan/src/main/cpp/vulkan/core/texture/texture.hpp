#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

#include "vulkan_context.h"

class Texture {
   public:
    Texture(VulkanContext& context, const std::string& path);
    ~Texture();

    [[nodiscard]] vk::ImageView getImageView() const;
    [[nodiscard]] vk::Sampler getSampler() const;

   private:
    VulkanContext& mContext;

    vk::UniqueImage mImage;
    vk::UniqueDeviceMemory mMemory;
    vk::UniqueImageView mImageView;
    vk::UniqueSampler mSampler;

    void loadImageFromFile(const std::string& path);
    void createImage(uint32_t width, uint32_t height);
    void createImageView();
    void createSampler();
};
