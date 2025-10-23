#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "render_pass.h"

class KaleidoscopePipelineConfig {

public:
    static vk::UniquePipeline createPipeline(VulkanContext& context, vk::PipelineLayout &pipelineLayout, RenderPass &renderPass);

private:
    static vk::PipelineVertexInputStateCreateInfo createVertexConfig();

    static std::vector<vk::PipelineShaderStageCreateInfo> createShaderStages(vk::ShaderModule& vertexModule, vk::ShaderModule& fragmentModule);
};
