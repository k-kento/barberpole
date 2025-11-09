#pragma once

#include "memory"
#include "vulkan/vulkan.hpp"
#include "drawing_renderer.hpp"
#include "ubo_buffer.hpp"
#include "command_buffer_helper.hpp"
#include "ubo_data.hpp"
#include "compute_descriptor.hpp"

class FrameContext {

public:
    FrameContext(VulkanContext &vkContext,
                 GraphicDescriptor &graphicDescriptor,
                 ComputeDescriptor &computeDescriptor) {
        mUboBuffer = std::make_unique<UboBuffer<UboData>>(vkContext);
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
        mDescriptorSet = graphicDescriptor.allocate(mUboBuffer->getBuffer(), sizeof(UboData));
        mComputeDescriptorSet = computeDescriptor.allocateDescriptorSet();
    }

    UboBuffer<UboData> *getUboBuffer() {
        return mUboBuffer.get();
    }

    vk::DescriptorSet getGraphicDescriptorSet() {
        return mDescriptorSet.get();
    }

    vk::DescriptorSet getComputeDescriptorSet() {
        return mComputeDescriptorSet.get();
    }

    vk::CommandBuffer getCommandBuffer() {
        return mCmdBuffer.get();
    }

    bool isFirst = true;

private:

    std::unique_ptr<UboBuffer<UboData>> mUboBuffer;
    vk::UniqueDescriptorSet mDescriptorSet;
    vk::UniqueDescriptorSet mComputeDescriptorSet;
    vk::UniqueCommandBuffer mCmdBuffer;

};
