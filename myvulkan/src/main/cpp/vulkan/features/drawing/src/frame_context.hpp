#pragma once

#include "command_buffer_helper.hpp"
#include "memory"
#include "uniform_buffer.hpp"
#include "ubo_data.hpp"
#include "vulkan/vulkan.hpp"

class FrameContext {
   public:
    FrameContext(VulkanContext& vkContext) {
        mUboBuffer = std::make_unique<UniformBuffer<UboData>>(vkContext);
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
    }

    UniformBuffer<UboData>* getUboBuffer() { return mUboBuffer.get(); }

    vk::CommandBuffer getCommandBuffer() { return mCmdBuffer.get(); }

   private:
    std::unique_ptr<UniformBuffer<UboData>> mUboBuffer;
    vk::UniqueCommandBuffer mCmdBuffer;
};
