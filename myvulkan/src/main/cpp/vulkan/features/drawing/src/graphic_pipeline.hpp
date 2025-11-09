#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "render_pass.h"
#include "graphic_descriptor.hpp"

class GraphicPipeline {

public:
    GraphicPipeline(VulkanContext &context, RenderPass &renderPass, GraphicDescriptor &graphicDescriptor);

    vk::PipelineLayout getLayout() const { return mPipelineLayout.get(); }

    vk::Pipeline getPipeline() const { return mPipeline.get(); }

private:
    std::vector<vk::VertexInputBindingDescription> mBindings;
    std::vector<vk::VertexInputAttributeDescription> mAttributes;
    vk::UniquePipelineLayout mPipelineLayout;
    vk::UniquePipeline mPipeline;

    vk::PipelineVertexInputStateCreateInfo createVertexConfig();

    vk::UniquePipeline createPipeline(VulkanContext &context,RenderPass &renderPass);

    std::vector<vk::PipelineShaderStageCreateInfo>
    createShaderStages(vk::ShaderModule &vertexModule, vk::ShaderModule &fragmentModule);
};
