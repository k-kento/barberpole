#pragma once

#include <vulkan/vulkan.hpp>
#include "render_pass.h"
#include "swap_chain.h"
#include "renderer_interface.hpp"

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

    vk::UniqueSemaphore mImageAvailable;
    vk::UniqueSemaphore mRenderFinished;
};
