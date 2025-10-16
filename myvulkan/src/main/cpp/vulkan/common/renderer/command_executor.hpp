#pragma once

#include <vulkan/vulkan.hpp>
#include "render_pass.h"
#include "swap_chain.h"
#include "render_strategy.hpp"

class CommandExecutor {
public:
    CommandExecutor(VulkanContext &context,
                    RenderPass &renderPass,
                    SwapChain &swapChain,
                    RenderStrategy &strategy);

    void renderFrame(
            SwapChain &swapChain,
            vk::Semaphore imageAvailable,
            vk::Semaphore renderFinished);

private:

    void recordCommandBuffers(RenderPass &renderPass, SwapChain &swapChain);

    static std::vector<vk::UniqueCommandBuffer>
    createCommandBuffers(vk::Device &device, vk::CommandPool &commandPool, size_t count);

    VulkanContext &mContext;
    RenderStrategy &mRendererStrategy;

    std::vector<vk::UniqueFramebuffer> mFrameBuffers;
    std::vector<vk::UniqueCommandBuffer> mCmdBuffers;
};
