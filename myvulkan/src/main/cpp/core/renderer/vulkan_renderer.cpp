#include "vulkan_renderer.h"
#include "log.h"
#include <chrono>
#include <iostream>
#include "vulkan_utils.h"

VulkanRenderer::VulkanRenderer(VulkanContext *vkContext, ANativeWindow *window) {
    mVkContext = vkContext;

    vk::Instance instance = mVkContext->getVkInstance();
    vk::PhysicalDevice physicalDevice = mVkContext->getVkPhysicalDevice();
    vk::Device device = mVkContext->getVkDevice();

    mSurface = std::make_unique<Surface>(mVkContext->getVkInstance(), window);
    mSwapChain = std::make_unique<SwapChain>(vkContext, mSurface->getSurface());
    mRenderPass = std::make_unique<RenderPass>(device, mSwapChain->getFormat());
    mFrameBuffer = VulkanUtils::createFrameBuffers(device, mSwapChain.get(), mRenderPass->getVkRenderPass());

    if (!createCommandPool(device, mVkContext->getPhysicalDevice()->getQueueFamilyIndex()))
        throw std::runtime_error("Failed to create command pool.");
    if (!createCommandBuffers(device)) throw std::runtime_error("Failed to create command buffers.");
    if (!createSemaphores()) throw std::runtime_error("Failed to create semaphores.");
}

VulkanRenderer::~VulkanRenderer() {
    VkDevice device = mVkContext->getVkDevice();
    stop();
    if (device) vkDeviceWaitIdle(device);  // GPU の処理完了を待機

    LOGI("VulkanRenderer destroyed");
}

// 描画スレッド開始
void VulkanRenderer::start() {
    if (gRun.load()) return;
    gRun.store(true);
    gRenderThread = std::thread(&VulkanRenderer::renderLoop, this);
}

// 描画スレッド停止
void VulkanRenderer::stop() {
    if (!gRun.load()) return;
    gRun.store(false);
    if (gRenderThread.joinable()) gRenderThread.join();
}

void VulkanRenderer::renderLoop() {
    auto device = mVkContext->getVkDevice();
    auto graphicsQueue = mVkContext->getDevice()->getGraphicsQueue();

    while (gRun.load()) {
        uint32_t imageIndex = 0;

        // 次に描画する SwapChain イメージを取得
        auto acquireResult = device.acquireNextImageKHR(
                mSwapChain->getSwapChain(),
                UINT64_MAX,
                mImageAvailable.get(),
                nullptr
        );
        if (acquireResult.result != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }
        imageIndex = acquireResult.value;


        renderFrame();

        // コマンドバッファをキューに流す
        vk::Semaphore waitSemaphores[] = {mImageAvailable.get()};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::Semaphore signalSemaphores[] = {mRenderFinished.get()};

        vk::SubmitInfo submitInfo{};
        submitInfo.setWaitSemaphores(waitSemaphores)
                .setWaitDstStageMask(waitStages)
                .setCommandBuffers(mCmdBuffers[imageIndex].get())
                .setSignalSemaphores(signalSemaphores);

        graphicsQueue.submit(submitInfo, nullptr);

        vk::SwapchainKHR swapchains[] = {mSwapChain->getSwapChain()};

        // 画面に描画結果を表示
        vk::PresentInfoKHR presentInfo{};
        presentInfo.setWaitSemaphores(signalSemaphores)
                .setSwapchains(swapchains)
                .setImageIndices(imageIndex);

        auto presentResult = graphicsQueue.presentKHR(presentInfo);
        if (presentResult != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to present swap chain image!");
        }

        // フレーム間の間隔を調整 (約60FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

bool VulkanRenderer::createCommandPool(vk::Device device, uint32_t queueFamilyIndex) {
    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    try {
        mCommandPool = device.createCommandPoolUnique(poolInfo);
    } catch (const vk::SystemError &e) {
        LOGE("Failed to create command pool: %s", e.what());
        return false;
    }
    return true;
}

bool VulkanRenderer::createCommandBuffers(vk::Device device) {
    // フレームごとのコマンドバッファを確保
    auto framebufferCount = mFrameBuffer.size();
    mCmdBuffers.resize(framebufferCount);

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = mCommandPool.get();
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(framebufferCount);

    try {
        std::vector<vk::UniqueCommandBuffer> uniqueBuffers =
                device.allocateCommandBuffersUnique(allocInfo);
        // gCmdBuffers に移動
        for (size_t i = 0; i < framebufferCount; ++i) {
            mCmdBuffers[i] = std::move(uniqueBuffers[i]);
        }
    } catch (const vk::SystemError &e) {
        LOGE("Failed to allocate command buffers: %s", e.what());
        return false;
    }

    return true;
}

// コマンドバッファに描画コマンドを記録
bool VulkanRenderer::recordCommandBuffers() {
    auto extent = mSwapChain->getExtent();
    const auto &framebuffers = mFrameBuffer;

    for (size_t i = 0; i < mCmdBuffers.size(); ++i) {
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

        LOGI("CommandBuffer[%zu] begin", i);
        mCmdBuffers[i]->begin(beginInfo);

        // クリアカラーの設定
        vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.5f, 0.0f, 1.0f});

        // RenderPass 開始情報
        vk::RenderPassBeginInfo rpBegin{};
        rpBegin.renderPass = mRenderPass->getVkRenderPass();
        rpBegin.framebuffer = framebuffers[i].get();
        rpBegin.renderArea.offset = vk::Offset2D{0, 0};
        rpBegin.renderArea.extent = extent;
        rpBegin.clearValueCount = 1;
        rpBegin.pClearValues = &clearColor;

        mCmdBuffers[i]->beginRenderPass(rpBegin, vk::SubpassContents::eInline);

        // ビューポート設定
        vk::Viewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        mCmdBuffers[i]->setViewport(0, viewport);

        // シザー矩形設定
        vk::Rect2D scissor{};
        scissor.offset = vk::Offset2D{0, 0};
        scissor.extent = extent;
        mCmdBuffers[i]->setScissor(0, scissor);

        // 実際の描画コマンドを記録
        recordDrawCommands(mCmdBuffers[i].get(), i);

        mCmdBuffers[i]->endRenderPass();
        mCmdBuffers[i]->end();
        LOGI("CommandBuffer[%zu] ended", i);
    }
    return true;
}

bool VulkanRenderer::createSemaphores() {
    auto device = mVkContext->getVkDevice();

    vk::SemaphoreCreateInfo semInfo{};

    try {
        mImageAvailable = device.createSemaphoreUnique(semInfo);
        mRenderFinished = device.createSemaphoreUnique(semInfo);
    } catch (const vk::SystemError &err) {
        LOGE("Failed to create semaphores: %s", err.what());
        return false;
    }

    return true;
}
