#include "vulkan_engine.hpp"
#include "log.h"

VulkanEngine::VulkanEngine(VulkanContext &vkContext,
                           std::unique_ptr<Surface> surface,
                           std::unique_ptr<SwapChain> swapChain,
                           std::unique_ptr<RenderPass> renderPass,
                           std::unique_ptr<RendererInterface> renderer)
        : mVkContext(vkContext),
          mSurface(std::move(surface)),
          mSwapChain(std::move(swapChain)),
          mRenderPass(std::move(renderPass)),
          mRenderer(std::move(renderer)) {

    vk::Device device = mVkContext.getVkDevice();

    mImageAvailable = device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});
    mRenderFinished = device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});

    mCommandExecutor = std::make_unique<CommandExecutor>(vkContext, *mRenderPass, *mSwapChain, *mRenderer);

    mRendererLooper = std::make_unique<RenderLooper>([this]() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - mLastTime).count();
        mLastTime = currentTime;

        float deltaTimeMs = static_cast<float>(deltaMs);

        mCommandExecutor->renderFrame(*mSwapChain, mImageAvailable.get(), mRenderFinished.get(), deltaTimeMs);
    });
}

VulkanEngine::~VulkanEngine() {
    vk::Device device = mVkContext.getVkDevice();
    stop();
    if (device) vkDeviceWaitIdle(device);  // GPU の処理完了を待機

    LOGI("VulkanEngine destroyed.");
}

void VulkanEngine::start() {
    mRendererLooper->start();
}

void VulkanEngine::stop() {
    mRendererLooper->stop();
}
