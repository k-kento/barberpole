#pragma once

#include <vulkan/vulkan.hpp>
#include <android/native_window.h>
#include <vector>
#include "vulkan_context.h"
#include "swap_chain.h"
#include "surface.h"
#include "render_pass.h"
#include "renderer_interface.hpp"
#include "render_looper.hpp"
#include "command_executor.hpp"

class VulkanEngine {
public:
    VulkanEngine(VulkanContext &vkContext,
                 std::unique_ptr<Surface> surface,
                 std::unique_ptr<SwapChain> swapChain,
                 std::unique_ptr<RenderPass> renderPass,
                 std::unique_ptr<RendererInterface> renderer);

    ~VulkanEngine();

    void start();

    void stop();

    void postTask(std::function<void()> task);

    void postMessage(std::unique_ptr<RenderMessage> message);

private:
    VulkanContext &mVkContext;

    std::unique_ptr<RendererInterface> mRenderer;

    std::unique_ptr<RenderLooper> mRendererLooper;

    std::unique_ptr<CommandExecutor> mCommandExecutor;

    std::unique_ptr<RenderPass> mRenderPass;
    std::unique_ptr<Surface> mSurface;
    std::unique_ptr<SwapChain> mSwapChain;

    std::chrono::high_resolution_clock::time_point mLastTime;

    void onRenderFrame();

    void onRenderMessage(std::unique_ptr<RenderMessage> message);
};
