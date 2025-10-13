#include "vulkan_engine.hpp"
#include "log.h"

VulkanEngine::VulkanEngine(VulkanContext &vkContext,
                           std::unique_ptr<Surface> surface,
                           std::unique_ptr<SwapChain> swapChain,
                           std::unique_ptr<RenderPass> renderPass,
                           std::unique_ptr<RenderStrategy> rendererStrategy)
        : mVkContext(vkContext),
          mSurface(std::move(surface)),
          mSwapChain(std::move(swapChain)),
          mRenderPass(std::move(renderPass)),
          mRendererStrategy(std::move(rendererStrategy)) {

    vk::Device device = mVkContext.getVkDevice();

    mImageAvailable = device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});
    mRenderFinished = device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});

    mCommandExecutor = std::make_unique<CommandExecutor>(vkContext, *mRenderPass, *mSwapChain, *mRendererStrategy);
    mRendererLooper = std::make_unique<RenderLooper>([this]() {
        mCommandExecutor->renderFrame(*mSwapChain, mImageAvailable.get(), mRenderFinished.get());
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
