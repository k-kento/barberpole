#pragma once

#include <vector>

#include "vulkan_context.h"
#include "render_pass.h"
#include "../graphic_descriptor.hpp"

class BasePipeline {

public:
    BasePipeline() = default;

    ~BasePipeline() = default;

    [[nodiscard]] vk::PipelineLayout getLayout() const { return mPipelineLayout.get(); }

    [[nodiscard]] vk::Pipeline getPipeline() const { return mPipeline.get(); }

    [[nodiscard]] vk::DescriptorSetLayout getDescriptorSetLayout() const { return mDescriptorSetLayout.get(); }

protected:
    std::vector<vk::VertexInputBindingDescription> mBindings;
    std::vector<vk::VertexInputAttributeDescription> mAttributes;
    vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    vk::UniquePipelineLayout mPipelineLayout;
    vk::UniquePipeline mPipeline;
};
