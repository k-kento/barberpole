#include "drawing_renderer.hpp"

#include "brush_change_message.hpp"
#include "device_buffer.h"
#include "frame_context.hpp"
#include "input_vertex.hpp"
#include "log.h"
#include "surface_changed_message.hpp"
#include "touch_message.hpp"
#include "ubo_buffer.hpp"
#include "ubo_data.hpp"
#include "view_bounds.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext& vkContext, std::unique_ptr<SurfaceContext> surfaceContext)
    : mVkContext(vkContext), mSurfaceContext(std::move(surfaceContext)) {
    mPipelineManager = std::make_unique<PipelineManager>(mVkContext, mSurfaceContext->getRenderPass());
    mBrushManager = std::make_unique<BrushManager>(mVkContext, *mPipelineManager);
    mStrokeManager = std::make_unique<StrokeManager>();

    // 初期ブラシを設定
    mStrokeManager->setCurrentBrush(mBrushManager->currentPtr());

    mFrameContexts.reserve(SurfaceContext::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
        mFrameContexts.emplace_back(std::make_unique<FrameContext>(vkContext));
    }
}

DrawingRenderer::~DrawingRenderer() = default;

void DrawingRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

    auto& frameContext = getCurrentFrameContext();
    auto cmdBuffer = frameContext.getCommandBuffer();

    beginFrame(frameContext, cmdBuffer);
    auto now = std::chrono::steady_clock::now();
    mElapsedTime = std::chrono::duration<float>(now - mStartTime).count();

    recordGraphicsPass(cmdBuffer, frameContext);
    endFrame(cmdBuffer, frameContext);
}

FrameContext& DrawingRenderer::getCurrentFrameContext() {
    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    return *mFrameContexts[currentFrameIndex];
}

void DrawingRenderer::beginFrame(FrameContext& frameContext, vk::CommandBuffer cmdBuffer) {
    updateUniforms(frameContext);
    mSurfaceContext->beginCommandBuffer(cmdBuffer);
}

void DrawingRenderer::updateUniforms(FrameContext& frameContext) {
    auto frameIndex = mSurfaceContext->getCurrentFrameIndex();
    mStrokeManager->setProjection(mProjection);
    mStrokeManager->updateTime(frameIndex, mElapsedTime);
}

void DrawingRenderer::recordGraphicsPass(vk::CommandBuffer cmdBuffer, FrameContext& frameContext) {
    auto frameIndex = mSurfaceContext->getCurrentFrameIndex();

    mSurfaceContext->beginRenderPass(cmdBuffer);

    // StrokeManager に描画を委譲
    mStrokeManager->onDraw(frameIndex, cmdBuffer);

    mSurfaceContext->endRenderPass(cmdBuffer);
}

void DrawingRenderer::endFrame(vk::CommandBuffer cmdBuffer, FrameContext& frameContext) {
    mSurfaceContext->endCommandBuffer(cmdBuffer);
    mSurfaceContext->submit(cmdBuffer);
    mSurfaceContext->present();
}

void DrawingRenderer::handleMessage(std::unique_ptr<RenderMessage> message) {
    if (auto* surfaceChangedMsg = dynamic_cast<const SurfaceChangedMessage*>(message.get())) {
        mViewBounds = surfaceChangedMsg->viewBounds;
        mProjection = surfaceChangedMsg->projection;
    } else if (auto touchMsg = dynamic_cast<const TouchMessage*>(message.get())) {
        switch (touchMsg->touchType) {
            case TouchMessage::Move: {
                auto normalizedX = touchMsg->x * mViewBounds.width() + mViewBounds.left;
                auto normalizedY = touchMsg->y * mViewBounds.height() - mViewBounds.top;
                mStrokeManager->onTouchMove(glm::vec2{normalizedX, normalizedY});
                break;
            }
            case TouchMessage::UP:
                mStrokeManager->endStroke();
                break;
            default:
                break;
        }
    } else if (auto brushChangeMsg = dynamic_cast<const BrushChangeMessage*>(message.get())) {
        mBrushManager->set(brushChangeMsg->brushType);
        mStrokeManager->setCurrentBrush(mBrushManager->currentPtr());
    }
}
