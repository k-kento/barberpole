#include "vulkan_renderer.h"
#include "log.h"
#include "render_pass.h"
#include <chrono>
#include <iostream>
#include <vulkan/vulkan_android.h>

VulkanRenderer::VulkanRenderer() {}

VulkanRenderer::~VulkanRenderer() { destroy(); }

bool VulkanRenderer::init(VulkanContext *vkContext, ANativeWindow *window) {
    mVkContext = vkContext;

    auto device = mVkContext->getDevice()->getDevice();

    mSurface = std::make_unique<Surface>();
    mSwapChain = std::make_unique<SwapChain>();
    mRenderPass = std::make_unique<RenderPass>(device);
    mFrameBuffer = std::make_unique<FrameBuffer>(device);

    if (!mSurface->create(mVkContext, window)) return false;
    if (!mSwapChain->init(mVkContext, mSurface->getSurface())) return false;
    if (!mRenderPass->init(mSwapChain->getFormat())) return false;
    if (!mFrameBuffer->init(mSwapChain.get(), mRenderPass.get())) return false;

    return true;
}

bool VulkanRenderer::postInit() {
    if (!createCommandPoolAndBuffers()) return false;
    if (!recordCommandBuffers()) return false;
    if (!createSemaphores()) return false;

    LOGI("Vulkan initialized");
    return true;
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

void VulkanRenderer::destroy() {
    VkDevice device = mVkContext->getDevice()->getDevice();
    stop();
    if (device != VK_NULL_HANDLE) vkDeviceWaitIdle(device);  // GPU の処理完了を待機
    if (gImageAvailable) vkDestroySemaphore(device, gImageAvailable, nullptr);
    if (gRenderFinished) vkDestroySemaphore(device, gRenderFinished, nullptr);
    if (gCommandPool) vkDestroyCommandPool(device, gCommandPool, nullptr);

    LOGI("VulkanRenderer destroyed");
}

void VulkanRenderer::renderLoop() {
    while (gRun.load()) {
        uint32_t imageIndex = 0;

        auto device = mVkContext->getDevice()->getDevice();
        auto graphicsQueue = mVkContext->getDevice()->getGraphicsQueue();

        // 次に描画する SwapChain イメージを取得
        vkAcquireNextImageKHR(device, mSwapChain->getSwapChain(), UINT64_MAX, gImageAvailable, VK_NULL_HANDLE,
                              &imageIndex);

        // コマンドバッファを GPU に送信
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {gImageAvailable};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &gCmdBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = {gRenderFinished};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

        // 画面に描画結果を表示
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR scs[] = {mSwapChain->getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = scs;
        presentInfo.pImageIndices = &imageIndex;
        vkQueuePresentKHR(graphicsQueue, &presentInfo);

        // フレーム間の間隔を調整 (約60FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

bool VulkanRenderer::createCommandPoolAndBuffers() {
    VkDevice device = mVkContext->getDevice()->getDevice();
    auto queueFamilyIndex = mVkContext->getPhysicalDevice()->getQueueFamilyIndex();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &gCommandPool) != VK_SUCCESS) {
        LOGE("Failed to init command pool");
        return false;
    }

    // フレームごとのコマンドバッファを確保
    gCmdBuffers.resize(mFrameBuffer.get()->getFrameBuffers().size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = gCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(gCmdBuffers.size());

    if (vkAllocateCommandBuffers(device, &allocInfo, gCmdBuffers.data()) != VK_SUCCESS) {
        LOGE("Failed to allocate command buffers");
        return false;
    }

    return true;
}

// コマンドバッファに描画コマンドを記録
bool VulkanRenderer::recordCommandBuffers() {
    for (size_t i = 0; i < gCmdBuffers.size(); ++i) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(gCmdBuffers[i], &beginInfo);

        // クリアカラーの設定
        VkClearValue clearColor = {1.0f, 1.0f, 1.0f, 1.0f};

        // RenderPass 開始情報を設定
        VkRenderPassBeginInfo rpBegin{};
        rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.renderPass = mRenderPass->getVkRenderPass();
        rpBegin.framebuffer = mFrameBuffer.get()->getFrameBuffers()[i];
        rpBegin.renderArea.offset = {0, 0};
        rpBegin.renderArea.extent = mSwapChain->getExtent();
        rpBegin.clearValueCount = 1;
        rpBegin.pClearValues = &clearColor;

        // RenderPass 開始
        vkCmdBeginRenderPass(gCmdBuffers[i], &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

        // ビューポート設定
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(mSwapChain->getExtent().width);
        viewport.height = static_cast<float>(mSwapChain->getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(gCmdBuffers[i], 0, 1, &viewport);

        // シザー矩形設定
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = mSwapChain->getExtent();
        vkCmdSetScissor(gCmdBuffers[i], 0, 1, &scissor);

        // 実際の描画コマンドを記録
        recordDrawCommands(gCmdBuffers[i], i);

        // RenderPass 終了
        vkCmdEndRenderPass(gCmdBuffers[i]);
        vkEndCommandBuffer(gCmdBuffers[i]);
    }
    return true;
}

// 描画用セマフォを作成
bool VulkanRenderer::createSemaphores() {
    VkDevice device = mVkContext->getDevice()->getDevice();
    VkSemaphoreCreateInfo semInfo{};
    semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &semInfo, nullptr, &gImageAvailable) != VK_SUCCESS) return false;
    if (vkCreateSemaphore(device, &semInfo, nullptr, &gRenderFinished) != VK_SUCCESS) return false;
    return true;
}
