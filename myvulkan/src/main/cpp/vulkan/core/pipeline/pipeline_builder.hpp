#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "colorblend/color_blend.hpp"

class PipelineBuilder {
   public:
    PipelineBuilder(const std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages,
                    const vk::PipelineVertexInputStateCreateInfo& vertexInputInfo,
                    const vk::RenderPass renderPass,
                    const vk::PipelineLayout pipelineLayout,
                    ColorBlend& colorBlend);

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
    vk::PipelineViewportStateCreateInfo viewportState{};
    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    vk::PipelineMultisampleStateCreateInfo multisampling{};
    vk::PipelineDepthStencilStateCreateInfo depthStencil{};
    vk::PipelineDynamicStateCreateInfo dynamicState{};
    std::vector<vk::DynamicState> dynamicStates;

    uint32_t subpass = 0;

   public:
    vk::UniquePipeline build(vk::Device device, vk::PipelineCache cache = nullptr);

   private:
    vk::PipelineLayout layout;
    vk::RenderPass mRenderPass;
    ColorBlend& colorBlend;
};
