#pragma once

#include <vulkan/vulkan.hpp>
#include <android/native_window.h>
#include <vector>
#include "vulkan_context.h"
#include "swap_chain.h"
#include "surface.h"
#include "render_pass.h"
#include "render_strategy.hpp"
#include "render_looper.hpp"
#include "command_executor.hpp"

class VulkanEngine {
public:
    VulkanEngine(VulkanContext &vkContext,
                 std::unique_ptr<Surface> surface,
                 std::unique_ptr<SwapChain> swapChain,
                 std::unique_ptr<RenderPass> renderPass,
                 std::unique_ptr<RenderStrategy> rendererStrategy);

    ~VulkanEngine();

    void start();

    void stop();

private:
    VulkanContext &mVkContext;

    std::unique_ptr<RenderStrategy> mRendererStrategy;

    std::unique_ptr<RenderLooper> mRendererLooper;

    std::unique_ptr<CommandExecutor> mCommandExecutor;

    std::unique_ptr<RenderPass> mRenderPass;
    std::unique_ptr<Surface> mSurface;
    std::unique_ptr<SwapChain> mSwapChain;

    vk::UniqueSemaphore mImageAvailable;
    vk::UniqueSemaphore mRenderFinished;
};
