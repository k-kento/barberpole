#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "render_pass.h"

class PipelineConfig {

public:
    vk::UniquePipeline
    createPipeline(VulkanContext &context, vk::PipelineLayout &pipelineLayout, RenderPass &renderPass);

private:
    std::vector<vk::VertexInputBindingDescription> mBindings;
    std::vector<vk::VertexInputAttributeDescription> mAttributes;

    vk::PipelineVertexInputStateCreateInfo createVertexConfig();

    std::vector<vk::PipelineShaderStageCreateInfo>
    createShaderStages(vk::ShaderModule &vertexModule, vk::ShaderModule &fragmentModule);
};
