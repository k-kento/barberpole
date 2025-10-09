#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <vulkan_context.h>

struct PipelineConfig {
    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
};

class GraphicsPipeline {
public:
    GraphicsPipeline(vk::Device device,
                     const PipelineConfig& pipelineConfig,
                     vk::RenderPass renderPass,
                     const std::vector<vk::ShaderModule> &shaderModules);

    [[nodiscard]] vk::Pipeline getPipeline() const { return mPipeline.get(); }

    [[nodiscard]] vk::PipelineLayout getPipelineLayout() const { return mPipelineLayout.get(); }

private:
    std::vector<vk::PipelineShaderStageCreateInfo>
    createShaderStages(const std::vector<vk::ShaderModule> &shaderModules);


    bool createGraphicsPipeline(vk::Device device, vk::RenderPass renderPass,
                                const std::vector<vk::ShaderModule> &shaderModules,
                                const PipelineConfig &pipelineConfig);

private:

    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
};
