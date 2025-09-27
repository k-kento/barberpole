#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <vulkan_context.h>

class VulkanContext; // 前方宣言

class GraphicsPipeline {
public:
    explicit GraphicsPipeline(VkDevice device);

    ~GraphicsPipeline();

    bool init(VkRenderPass renderPass, const std::vector<VkShaderModule> &shaderModules);

    [[nodiscard]] VkPipeline getPipeline() const { return mPipeline; }

    [[nodiscard]] VkPipelineLayout getPipelineLayout() const { return mPipelineLayout; }

private:
    bool createPipelineLayout();

    bool createGraphicsPipeline(VkRenderPass renderPass, const std::vector<VkShaderModule> &shaderModules);

    static std::vector<VkPipelineShaderStageCreateInfo> createShaderStages(
            const std::vector<VkShaderModule> &shaderModules);

    VkPipeline mPipeline{VK_NULL_HANDLE};
    VkPipelineLayout mPipelineLayout{VK_NULL_HANDLE};
    VkDevice mDevice{VK_NULL_HANDLE};
};
