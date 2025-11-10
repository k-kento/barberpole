#pragma once

#include "memory"
#include "vulkan/vulkan.hpp"
#include "drawing_renderer.hpp"
#include "ubo_buffer.hpp"
#include "command_buffer_helper.hpp"
#include "ubo_data.hpp"
#include "compute_descriptor.hpp"
#include "vertex_buffer.hpp"

class FrameContext {

public:
    FrameContext(VulkanContext &vkContext,
                 GraphicDescriptor &graphicDescriptor,
                 ComputeDescriptor &computeDescriptor) {
        mUboBuffer = std::make_unique<UboBuffer<UboData>>(vkContext);
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
        mDescriptorSet = graphicDescriptor.allocate(mUboBuffer->getBuffer(), sizeof(UboData));
        mComputeDescriptorSet = computeDescriptor.allocateDescriptorSet();
        mVertexBuffer = std::make_unique<VertexBuffer>(vkContext);
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

    VertexBuffer *getVertexBuffer() {
        return mVertexBuffer.get();
    }

    bool initialized = false;

private:

    std::unique_ptr<UboBuffer<UboData>> mUboBuffer;
    vk::UniqueDescriptorSet mDescriptorSet;
    vk::UniqueDescriptorSet mComputeDescriptorSet;
    vk::UniqueCommandBuffer mCmdBuffer;
    std::unique_ptr<VertexBuffer> mVertexBuffer;

};
