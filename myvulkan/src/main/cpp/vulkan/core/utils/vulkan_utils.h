#pragma once

#include <android/asset_manager.h>
#include <vulkan/vulkan.h>

#include <vector>
#include <vulkan/vulkan.hpp>

#include "swap_chain.hpp"
#include "view_bounds.hpp"

class VulkanUtils {
   public:
    VulkanUtils() = default;

    ~VulkanUtils() = default;

    static std::vector<char> readTextFile(AAssetManager* assetManager, const std::string& filename);

    static std::vector<unsigned char> readBinaryFile(AAssetManager* manager, const std::string& filename);

    static std::vector<vk::UniqueFramebuffer> createFrameBuffers(vk::Device device,
                                                                 SwapChain* swapChain,
                                                                 vk::RenderPass renderPass);

    static glm::mat4 generateProjectionMatrix(uint32_t deviceRotationDegree, ViewBounds& viewBounds);
};
