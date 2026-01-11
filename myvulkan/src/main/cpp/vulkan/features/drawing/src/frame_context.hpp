#pragma once

#include "memory"
#include "vulkan/vulkan.hpp"
#include "ubo_buffer.hpp"
#include "command_buffer_helper.hpp"
#include "ubo_data.hpp"

class FrameContext {

public:
    FrameContext(VulkanContext &vkContext) {
        mUboBuffer = std::make_unique<UboBuffer<UboData>>(vkContext);
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
    }

    UboBuffer<UboData> *getUboBuffer() {
        return mUboBuffer.get();
    }

    vk::CommandBuffer getCommandBuffer() {
        return mCmdBuffer.get();
    }

private:

    std::unique_ptr<UboBuffer<UboData>> mUboBuffer;
    vk::UniqueCommandBuffer mCmdBuffer;

};
