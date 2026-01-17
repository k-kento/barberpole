#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "compute_descriptor.hpp"
#include "shader_helper.hpp"
#include "vulkan_context.h"

class ComputePipeline {
   public:
    ComputePipeline(VulkanContext& vkContext, ComputeDescriptor& descriptor) {
        vk::PushConstantRange pushRange{};
        pushRange.stageFlags = vk::ShaderStageFlagBits::eCompute;
        pushRange.offset = 0;
        pushRange.size = sizeof(uint32_t);

        vk::PipelineLayoutCreateInfo layoutInfo{};
        std::array<vk::DescriptorSetLayout, 1> layouts = {descriptor.getLayout()};
        layoutInfo.setSetLayouts(layouts);
        layoutInfo.setPushConstantRanges(pushRange);

        mPipelineLayout = vkContext.getDevice().createPipelineLayoutUnique(layoutInfo);

        auto shaderModule = ShaderHelper::createShaderModule(vkContext, "shaders/drawing/brush.comp.spv");

        // Compute shader ステージ情報
        vk::PipelineShaderStageCreateInfo stageInfo{};
        stageInfo.stage = vk::ShaderStageFlagBits::eCompute;
        stageInfo.module = *shaderModule;
        stageInfo.pName = "main";

        // パイプライン作成
        vk::ComputePipelineCreateInfo pipelineInfo{};
        pipelineInfo.stage = stageInfo;
        pipelineInfo.layout = mPipelineLayout.get();

        auto result = vkContext.getDevice().createComputePipelineUnique(nullptr, pipelineInfo);
        if (result.result != vk::Result::eSuccess) {
            throw std::runtime_error("ComputePipeline: createComputePipelineUnique failed.");
        }
        mPipeline = std::move(result.value);
    }

    vk::PipelineLayout getLayout() const { return mPipelineLayout.get(); }

    vk::Pipeline getPipeline() const { return mPipeline.get(); }

   private:
    vk::UniquePipelineLayout mPipelineLayout;
    vk::UniquePipeline mPipeline;
};
