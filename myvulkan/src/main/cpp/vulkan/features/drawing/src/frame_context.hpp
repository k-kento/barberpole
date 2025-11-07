#pragma once

#include "memory"
#include "vulkan/vulkan.hpp"
#include "drawing_renderer.hpp"
#include "instance_buffer.hpp"
#include "ubo_buffer.hpp"
#include "command_buffer_helper.hpp"

class FrameContext {

public:
    struct UboData {
        glm::mat4 projection;
    };

    FrameContext(VulkanContext &vkContext, Descriptor &descriptor) {
        mInstanceBuffer = std::make_unique<InstanceBuffer>(vkContext);
        mUboBuffer = std::make_unique<UboBuffer<UboData>>(vkContext);
        mDescriptorSet = descriptor.allocate(mUboBuffer->getBuffer(), sizeof(UboData));
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
    }

    InstanceBuffer *getInstanceBuffer() {
        return mInstanceBuffer.get();
    }

    UboBuffer<UboData> *getUboBuffer() {
        return mUboBuffer.get();
    }

    vk::DescriptorSet getDescriptorSet() {
        return mDescriptorSet.get();
    }

    vk::CommandBuffer getCommandBuffer() {
        return mCmdBuffer.get();
    }

private:

    std::unique_ptr<InstanceBuffer> mInstanceBuffer;
    std::unique_ptr<UboBuffer<UboData>> mUboBuffer;
    vk::UniqueDescriptorSet mDescriptorSet;
    vk::UniqueCommandBuffer mCmdBuffer;
};
