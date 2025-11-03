#include "surface_context.hpp"

SurfaceContext::SurfaceContext(VulkanContext &vkContext, std::shared_ptr<Surface> surface) :
        mVkContext(vkContext),
        mSurface(std::move(surface)) {

    mSwapChain = std::make_unique<SwapChain>(vkContext, mSurface->getSurface());
    mRenderPass = std::make_unique<RenderPass>(vkContext.getDevice(), mSwapChain->getFormat());

    mFrameContexts.reserve(MAX_FRAMES_IN_FLIGHT);
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        auto frame = std::make_unique<FrameContext>(mVkContext);
        mFrameContexts.push_back(std::move(frame));
    }

    mFrameBuffers = createFrameBuffers();
}

FrameContext *SurfaceContext::getFrameContext() {
    return mFrameContexts[mCurrentFrameIndex].get();
}

void SurfaceContext::acquireNextImage() {
    auto device = mVkContext.getDevice();
    auto frameContext = mFrameContexts[mCurrentFrameIndex].get();
    auto fence = frameContext->getInFlightFences();

    device.waitForFences(1, &fence, VK_TRUE, UINT64_MAX);

    mSwapChain->acquireNextImage(frameContext->getImageAvailable());

    device.resetFences(1, &fence);
}

void SurfaceContext::submit() {
    auto frameContext = mFrameContexts[mCurrentFrameIndex].get();
    vk::Semaphore imageAvailable = frameContext->getImageAvailable();
    vk::Semaphore renderFinished = frameContext->getRenderFinished();
    vk::CommandBuffer commandBuffer = frameContext->getCommandBuffer();

    // 描画の最終段階で同期
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    // コマンドバッファを GPU キューに送信
    vk::SubmitInfo submitInfo{};
    submitInfo.setWaitSemaphores(imageAvailable)
            .setWaitDstStageMask(waitStages)
            .setSignalSemaphores(renderFinished)
            .setCommandBuffers(commandBuffer);

    mVkContext.getGraphicsQueue().submit(submitInfo, frameContext->getInFlightFences());
}

void SurfaceContext::present() {
    auto frameContext = mFrameContexts[mCurrentFrameIndex].get();
    mSwapChain->present(frameContext->getRenderFinished());
    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void SurfaceContext::recordCommandBuffers(std::function<void()> recordDraw) {
    auto frameContext = mFrameContexts[mCurrentFrameIndex].get();
    auto cmdBuffer = frameContext->getCommandBuffer();
    auto currentImageIndex = mSwapChain->getCurrentImageIndex();

    auto extent = mSwapChain->getExtent();

    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
    cmdBuffer.begin(beginInfo);

    vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    vk::RenderPassBeginInfo rpBegin{};
    rpBegin.renderPass = mRenderPass->getVkRenderPass();
    rpBegin.framebuffer = mFrameBuffers[currentImageIndex].get();
    rpBegin.renderArea.extent = mSwapChain->getExtent();
    rpBegin.clearValueCount = 1;
    rpBegin.pClearValues = &clearColor;

    cmdBuffer.beginRenderPass(rpBegin, vk::SubpassContents::eInline);

    vk::Viewport viewport{0, 0, (float) extent.width, (float) extent.height, 0.0f, 1.0f};
    cmdBuffer.setViewport(0, viewport);
    vk::Rect2D scissor{{0, 0}, extent};
    cmdBuffer.setScissor(0, scissor);

    recordDraw();

    cmdBuffer.endRenderPass();
    cmdBuffer.end();
}

std::vector<vk::UniqueFramebuffer> SurfaceContext::createFrameBuffers() {
    std::vector<vk::UniqueFramebuffer> frameBuffers;

    auto imageViews = mSwapChain->getImageViews();
    frameBuffers.reserve(imageViews.size());

    for (auto &imageView: imageViews) {
        vk::FramebufferCreateInfo fbInfo(
                {},
                mRenderPass->getVkRenderPass(),
                1,
                &imageView,
                mSwapChain->getExtent().width,
                mSwapChain->getExtent().height,
                1
        );

        try {
            frameBuffers.push_back(mVkContext.getDevice().createFramebufferUnique(fbInfo));
        } catch (const vk::SystemError &e) {
            throw std::runtime_error(std::string("Failed to create framebuffer: ") + e.what());
        }
    }

    LOGI("Frame Buffers created.");

    return frameBuffers;
}
