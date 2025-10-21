#include "command_executor.hpp"
#include "vulkan_utils.h"

CommandExecutor::CommandExecutor(VulkanContext &context,
                                 RenderPass &renderPass,
                                 SwapChain &swapChain,
                                 RendererInterface &renderer)
        : mContext(context), mRenderer(renderer) {

    uint32_t queueFamilyIndex = context.getPhysicalDeviceBundle().queueFamilyIndex;
    auto device = context.getDevice();

    vk::SemaphoreCreateInfo semaphoreInfo{};
    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled; // フェンスの初期状態をシグナルにする

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        mImageAvailable[i] = device.createSemaphoreUnique(semaphoreInfo);
        mRenderFinished[i] = device.createSemaphoreUnique(semaphoreInfo);
        mInFlightFences[i] = device.createFenceUnique(fenceInfo);
    }

    mFrameBuffers = VulkanUtils::createFrameBuffers(device, &swapChain, renderPass.getVkRenderPass());
    auto commandPool = mContext.getGraphicsCommandPool();
    mCmdBuffers = createCommandBuffers(device, commandPool, mFrameBuffers.size());

    recordCommandBuffers(renderPass, swapChain);
}

void CommandExecutor::renderFrame(SwapChain &swapChain, float deltaTimeMs) {
    auto device = mContext.getDevice();
    auto graphicsQueue = mContext.getGraphicsQueue();
    auto swapChainKHR = swapChain.getSwapChain();

    uint32_t currentFrame = mCurrentFrame % MAX_FRAMES_IN_FLIGHT;
    mCurrentFrame++;

    // 次に表示可能な画像のインデックスを取得する
    auto acquireResult = device.acquireNextImageKHR(
            swapChainKHR,
            UINT64_MAX,
            mImageAvailable[currentFrame].get(),
            nullptr
    );

    uint32_t imageIndex = acquireResult.value;

    // 前フレームが GPU 完了するのを待つ
    device.waitForFences(1, &mInFlightFences[currentFrame].get(), VK_TRUE, UINT64_MAX);
    device.resetFences(1, &mInFlightFences[currentFrame].get());

    mRenderer.renderFrame(deltaTimeMs);

    // 描画の最終段階で同期
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    // コマンドバッファを GPU キューに送信
    vk::SubmitInfo submitInfo{};
    submitInfo.setWaitSemaphores(mImageAvailable[currentFrame].get())
            .setWaitDstStageMask(waitStages)
            .setCommandBuffers(mCmdBuffers[imageIndex].get())
            .setSignalSemaphores(mRenderFinished[currentFrame].get());

    graphicsQueue.submit(submitInfo, mInFlightFences[currentFrame].get());

    // GPU が描画完了したフレームを表示
    vk::SwapchainKHR swapChains[] = {swapChainKHR};
    vk::PresentInfoKHR presentInfo{};
    presentInfo.setWaitSemaphores(mRenderFinished[currentFrame].get())
            .setSwapchains(swapChains)
            .setImageIndices(imageIndex);

    graphicsQueue.presentKHR(presentInfo);
}

void CommandExecutor::recordCommandBuffers(RenderPass &renderPass, SwapChain &swapChain) {
    auto extent = swapChain.getExtent();
    for (size_t i = 0; i < mCmdBuffers.size(); ++i) {
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        mCmdBuffers[i]->begin(beginInfo);

        vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        vk::RenderPassBeginInfo rpBegin{};
        rpBegin.renderPass = renderPass.getVkRenderPass();
        rpBegin.framebuffer = mFrameBuffers[i].get();
        rpBegin.renderArea.extent = extent;
        rpBegin.clearValueCount = 1;
        rpBegin.pClearValues = &clearColor;

        mCmdBuffers[i]->beginRenderPass(rpBegin, vk::SubpassContents::eInline);

        vk::Viewport viewport{0, 0, (float) extent.width, (float) extent.height, 0.0f, 1.0f};
        mCmdBuffers[i]->setViewport(0, viewport);
        vk::Rect2D scissor{{0, 0}, extent};
        mCmdBuffers[i]->setScissor(0, scissor);

        mRenderer.recordDrawCommands(mCmdBuffers[i].get());

        mCmdBuffers[i]->endRenderPass();
        mCmdBuffers[i]->end();
    }
}

std::vector<vk::UniqueCommandBuffer>
CommandExecutor::createCommandBuffers(vk::Device &device, vk::CommandPool &commandPool, size_t count) {
    std::vector<vk::UniqueCommandBuffer> cmdBuffers;

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    try {
        cmdBuffers = device.allocateCommandBuffersUnique(allocInfo);
    } catch (const vk::SystemError &e) {
        throw std::runtime_error(std::string("Failed to allocate command buffers: ") + e.what());
    }

    return cmdBuffers;
}
