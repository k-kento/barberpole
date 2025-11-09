#include "surface_context.hpp"

SurfaceContext::SurfaceContext(VulkanContext &vkContext,
                               std::shared_ptr<Surface> surface) :
        mVkContext(vkContext),
        mSurface(std::move(surface)) {

    mSwapChain = std::make_unique<SwapChain>(vkContext, mSurface->getSurface());
    mRenderPass = std::make_unique<RenderPass>(vkContext.getDevice(), mSwapChain->getFormat());
    mFrameBuffers = createFrameBuffers();

    vk::FenceCreateInfo fenceInfo{ vk::FenceCreateFlagBits::eSignaled };

    mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        mInFlightFences[i] = mVkContext.getDevice().createFenceUnique(fenceInfo);
    }
}

void SurfaceContext::acquireNextImage() {
    auto device = mVkContext.getDevice();
    auto inFlightFence = mInFlightFences[mCurrentFrameIndex].get();

    device.waitForFences(1, &inFlightFence, VK_TRUE, UINT64_MAX);
    mSwapChain->acquireNextImage();
    device.resetFences(1, &inFlightFence);
}

void SurfaceContext::submit(vk::CommandBuffer commandBuffer) {
    auto currentImageIndex = mSwapChain->getCurrentImageIndex();
    auto imageAvailable = mSwapChain->getImageAvailable();
    auto renderFinished = mSwapChain->getRenderFinished();

    // 描画の最終段階で同期
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    // コマンドバッファを GPU キューに送信
    vk::SubmitInfo submitInfo{};
    submitInfo.setWaitSemaphores(imageAvailable)
            .setWaitDstStageMask(waitStages)
            .setSignalSemaphores(renderFinished)
            .setCommandBuffers(commandBuffer);

    mVkContext.getGraphicsQueue().submit(submitInfo, mInFlightFences[mCurrentFrameIndex].get());
}

void SurfaceContext::present() {
    mSwapChain->present();
    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void SurfaceContext::beginCommandBuffer(vk::CommandBuffer cmdBuffer) {
    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
    cmdBuffer.begin(beginInfo);
}

void SurfaceContext::beginRenderPass(vk::CommandBuffer cmdBuffer) {
    auto currentImageIndex = mSwapChain->getCurrentImageIndex();
    auto extent = mSwapChain->getExtent();

    vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

    vk::RenderPassBeginInfo rpBegin{};
    rpBegin.renderPass = mRenderPass->getVkRenderPass();
    rpBegin.framebuffer = mFrameBuffers[currentImageIndex].get();
    rpBegin.renderArea.extent = extent;
    rpBegin.clearValueCount = 1;
    rpBegin.pClearValues = &clearColor;

    cmdBuffer.beginRenderPass(rpBegin, vk::SubpassContents::eInline);

    vk::Viewport viewport{0, 0, (float) extent.width, (float) extent.height, 0.0f, 1.0f};
    vk::Rect2D scissor{{0, 0}, extent};
    cmdBuffer.setViewport(0, viewport);
    cmdBuffer.setScissor(0, scissor);
}

void SurfaceContext::endRenderPass(vk::CommandBuffer cmdBuffer) {
    cmdBuffer.endRenderPass();
}

void SurfaceContext::endCommandBuffer(vk::CommandBuffer cmdBuffer) {
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
