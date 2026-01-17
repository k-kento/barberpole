#pragma once

#include <vulkan/vulkan.hpp>

class Rasterizer {
   public:
    static vk::PipelineRasterizationStateCreateInfo get() {
        return vk::PipelineRasterizationStateCreateInfo{}
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setCullMode(vk::CullModeFlagBits::eNone)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);
    }
};
