#pragma once

#include <vulkan/vulkan.hpp>
#include "render_pass.h"
#include "swap_chain.hpp"
#include "renderer_interface.hpp"

class FrameContext {
public:
    FrameContext(VulkanContext &vkContext);

    [[nodiscard]] vk::Fence getInFlightFences() const {
        return mInFlightFences.get();
    }

    [[nodiscard]] vk::Semaphore getImageAvailable() const {
        return mImageAvailable.get();
    }

    [[nodiscard]] vk::Semaphore getRenderFinished() const {
        return mRenderFinished.get();
    }

    [[nodiscard]] vk::CommandBuffer getCommandBuffer() {
        return mCommandBuffer.get();
    }

private:

    VulkanContext &mVkContext;

    vk::UniqueFence mInFlightFences;
    vk::UniqueSemaphore mImageAvailable;
    vk::UniqueSemaphore mRenderFinished;
    vk::UniqueCommandBuffer mCommandBuffer;

    vk::UniqueCommandBuffer createCommandBuffer();
};
