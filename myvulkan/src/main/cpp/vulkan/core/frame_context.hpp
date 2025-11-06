#pragma once

#include <vulkan/vulkan.hpp>
#include "render_pass.h"
#include "swap_chain.hpp"
#include "renderer.hpp"

class FrameContext {
public:
    FrameContext(VulkanContext &vkContext);

    virtual ~FrameContext() = default;

    [[nodiscard]] vk::Fence getInFlightFences() const {
        return mInFlightFences.get();
    }

    [[nodiscard]] vk::CommandBuffer getCommandBuffer() {
        return mCommandBuffer.get();
    }

private:

    VulkanContext &mVkContext;

    vk::UniqueFence mInFlightFences;
    vk::UniqueCommandBuffer mCommandBuffer;

    vk::UniqueCommandBuffer createCommandBuffer();
};
