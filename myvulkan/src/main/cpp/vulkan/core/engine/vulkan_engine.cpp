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

    vk::Device device = mVkContext.getDevice();

    mCommandExecutor = std::make_unique<CommandExecutor>(vkContext, *mRenderPass, *mSwapChain, *mRenderer);

    mRendererLooper = std::make_unique<RenderLooper>(
            [this]() { this->onRenderFrame(); },
            [this](std::unique_ptr<RenderMessage> msg) { this->onRenderMessage(std::move(msg)); }
    );
}

VulkanEngine::~VulkanEngine() {
    vk::Device device = mVkContext.getDevice();
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

void VulkanEngine::postTask(std::function<void()> task) {
    mRendererLooper->postTask(task);
}

void VulkanEngine::postMessage(std::unique_ptr<RenderMessage> message) {
    mRendererLooper->postMessage(std::move(message));
}

void VulkanEngine::onRenderFrame() {
    // 経過時間計算
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTimeMs = std::chrono::duration<float, std::milli>(now - mLastTime).count();
    mLastTime = now;

    mCommandExecutor->renderFrame(*mSwapChain, deltaTimeMs);
}

void VulkanEngine::onRenderMessage(std::unique_ptr<RenderMessage> message) {
    mRenderer->handleMessage(std::move(message));
}
