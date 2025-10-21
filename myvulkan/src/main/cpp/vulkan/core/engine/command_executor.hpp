#pragma once

#include <vulkan/vulkan.hpp>
#include "render_pass.h"
#include "swap_chain.h"
#include "renderer_interface.hpp"

namespace {
    const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
};

class CommandExecutor {
public:
    CommandExecutor(VulkanContext &context,
                    RenderPass &renderPass,
                    SwapChain &swapChain,
                    RendererInterface &renderer);

    void renderFrame(SwapChain &swapChain, float deltaTimeMs);

private:

    void recordCommandBuffers(RenderPass &renderPass, SwapChain &swapChain);

    static std::vector<vk::UniqueCommandBuffer>
    createCommandBuffers(vk::Device &device, vk::CommandPool &commandPool, size_t count);

    VulkanContext &mContext;
    RendererInterface &mRenderer;

    std::vector<vk::UniqueFramebuffer> mFrameBuffers;
    std::vector<vk::UniqueCommandBuffer> mCmdBuffers;

    std::array<vk::UniqueFence, MAX_FRAMES_IN_FLIGHT> mInFlightFences;
    std::array<vk::UniqueSemaphore, MAX_FRAMES_IN_FLIGHT> mImageAvailable;
    std::array<vk::UniqueSemaphore, MAX_FRAMES_IN_FLIGHT> mRenderFinished;
    uint32_t mCurrentFrame = 0;
};
