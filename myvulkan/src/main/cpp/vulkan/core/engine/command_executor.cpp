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

    mImageAvailable.reserve(mRenderer.getMaxFramesInFlight());
    mRenderFinished.reserve(mRenderer.getMaxFramesInFlight());
    mInFlightFences.reserve(mRenderer.getMaxFramesInFlight());

    for (size_t i = 0; i < mRenderer.getMaxFramesInFlight(); i++) {
        mImageAvailable.push_back(device.createSemaphoreUnique(semaphoreInfo));
        mRenderFinished.push_back(device.createSemaphoreUnique(semaphoreInfo));
        mInFlightFences.push_back(device.createFenceUnique(fenceInfo));
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

    uint32_t frameIndex = mFrameIndex % mRenderer.getMaxFramesInFlight();
    mFrameIndex++;

    auto imageAvailable = mImageAvailable[frameIndex].get();
    auto renderFinished = mRenderFinished[frameIndex].get();
    auto inFlightFence = mInFlightFences[frameIndex].get();

    // 前フレームが GPU 完了するのを待つ
    device.waitForFences(1, &inFlightFence, VK_TRUE, UINT64_MAX);
    device.resetFences(1, &inFlightFence);

    // 次に表示可能な画像のインデックスを取得する
    auto acquireResult = device.acquireNextImageKHR(
            swapChainKHR,
            UINT64_MAX,
            imageAvailable,
            nullptr
    );

    if (acquireResult.result == vk::Result::eErrorOutOfDateKHR) {
        // TODO リサイズなど スワップチェーン作り直し
        return;
    }

    uint32_t imageIndex = acquireResult.value;

    mRenderer.renderFrame(deltaTimeMs, frameIndex);

    // 描画の最終段階で同期
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    // コマンドバッファを GPU キューに送信
    vk::SubmitInfo submitInfo{};
    submitInfo.setWaitSemaphores(imageAvailable)
            .setWaitDstStageMask(waitStages)
            .setCommandBuffers(mCmdBuffers[imageIndex].get())
            .setSignalSemaphores(renderFinished);

    graphicsQueue.submit(submitInfo, inFlightFence);

    // GPU が描画完了したフレームを表示
    vk::SwapchainKHR swapChains[] = {swapChainKHR};
    vk::PresentInfoKHR presentInfo{};
    presentInfo.setWaitSemaphores(renderFinished)
            .setSwapchains(swapChains)
            .setImageIndices(imageIndex);

    graphicsQueue.presentKHR(presentInfo);
}

void CommandExecutor::recordCommandBuffers(RenderPass &renderPass, SwapChain &swapChain) {
    auto extent = swapChain.getExtent();
    for (size_t imageIndex = 0; imageIndex < mCmdBuffers.size(); ++imageIndex) {
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        mCmdBuffers[imageIndex]->begin(beginInfo);

        vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        vk::RenderPassBeginInfo rpBegin{};
        rpBegin.renderPass = renderPass.getVkRenderPass();
        rpBegin.framebuffer = mFrameBuffers[imageIndex].get();
        rpBegin.renderArea.extent = extent;
        rpBegin.clearValueCount = 1;
        rpBegin.pClearValues = &clearColor;

        mCmdBuffers[imageIndex]->beginRenderPass(rpBegin, vk::SubpassContents::eInline);

        vk::Viewport viewport{0, 0, (float) extent.width, (float) extent.height, 0.0f, 1.0f};
        mCmdBuffers[imageIndex]->setViewport(0, viewport);
        vk::Rect2D scissor{{0, 0}, extent};
        mCmdBuffers[imageIndex]->setScissor(0, scissor);

        // CommandBuffer 記録時点では実際のフレーム番号は不明
        // 各 SwapChain イメージに対して MaxFramesInFlight 分の DescriptorSet を割り当て
        uint32_t frameIndex = imageIndex % mRenderer.getMaxFramesInFlight();
        mRenderer.recordDrawCommands(mCmdBuffers[imageIndex].get(), frameIndex);

        mCmdBuffers[imageIndex]->endRenderPass();
        mCmdBuffers[imageIndex]->end();
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
