#pragma once

#include "render_pass.h"
#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

class KaleidoscopePipelineConfig {
   public:
    vk::UniquePipeline createPipeline(VulkanContext& context,
                                      vk::PipelineLayout& pipelineLayout,
                                      RenderPass& renderPass);

   private:
    vk::PipelineVertexInputStateCreateInfo createVertexConfig();

    std::vector<vk::PipelineShaderStageCreateInfo> createShaderStages(vk::ShaderModule& vertexModule,
                                                                      vk::ShaderModule& fragmentModule);

    std::vector<vk::VertexInputBindingDescription> mBindings;
    std::vector<vk::VertexInputAttributeDescription> mAttributes;
};
