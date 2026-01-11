#include "drawing_renderer.hpp"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "touch_message.hpp"
#include "log.h"
#include "surface_changed_message.hpp"
#include "frame_context.hpp"
#include "input_vertex.hpp"
#include "ubo_data.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surfaceContext) :
        mVkContext(vkContext),
        mSurfaceContext(std::move(surfaceContext)) {
    auto device = mVkContext.getDevice();

    mStroke = std::make_unique<Stroke>();
    mPipelineManager = std::make_unique<PipelineManager>(mVkContext, mSurfaceContext->getRenderPass());
    mBrushManager = std::make_unique<BrushManager>(mVkContext, *mPipelineManager);

    mFrameContexts.reserve(SurfaceContext::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
        mFrameContexts.emplace_back(std::make_unique<FrameContext>(vkContext));
    }
}

DrawingRenderer::~DrawingRenderer() = default;

void DrawingRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

    auto &frameContext = getCurrentFrameContext();
    auto cmdBuffer = frameContext.getCommandBuffer();

    beginFrame(frameContext, cmdBuffer);
    auto now = std::chrono::steady_clock::now();
    mElapsedTime = std::chrono::duration<float>(now - mStartTime).count();

    recordGraphicsPass(cmdBuffer, frameContext);
    endFrame(cmdBuffer, frameContext);
}

FrameContext &DrawingRenderer::getCurrentFrameContext() {
    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    return *mFrameContexts[currentFrameIndex];
}

void DrawingRenderer::beginFrame(FrameContext &frameContext, vk::CommandBuffer cmdBuffer) {
    updateUniforms(frameContext);
    mSurfaceContext->beginCommandBuffer(cmdBuffer);
}

void DrawingRenderer::updateUniforms(FrameContext &frameContext) {
    mBrushManager->current().setProjection(mProjection);
}

void DrawingRenderer::recordGraphicsPass(vk::CommandBuffer cmdBuffer,
                                         FrameContext &frameContext) {
    auto frameIndex = mSurfaceContext->getCurrentFrameIndex();

    mSurfaceContext->beginRenderPass(cmdBuffer);

    Brush& brush = mBrushManager->current();
    brush.applyStroke(*mStroke, frameIndex);
    brush.record(cmdBuffer, frameIndex);

    mSurfaceContext->endRenderPass(cmdBuffer);
}

void DrawingRenderer::endFrame(vk::CommandBuffer cmdBuffer, FrameContext &frameContext) {
    mSurfaceContext->endCommandBuffer(cmdBuffer);
    mSurfaceContext->submit(cmdBuffer);
    mSurfaceContext->present();
}

void DrawingRenderer::handleMessage(std::unique_ptr<RenderMessage> message) {
    if (auto *surfaceChangedMsg = dynamic_cast<const SurfaceChangedMessage *>(message.get())) {
        mViewBounds = surfaceChangedMsg->viewBounds;
        mProjection = surfaceChangedMsg->projection;
    } else if (auto touchMsg = dynamic_cast<const TouchMessage *>(message.get())) {
        switch (touchMsg->touchType) {
            case TouchMessage::Move: {
                auto normalizedX = touchMsg->x * mViewBounds.width() + mViewBounds.left;
                auto normalizedY = touchMsg->y * mViewBounds.height() - mViewBounds.top;
                auto point = InputVertex{glm::vec2{normalizedX, normalizedY}};
                mStroke->addPoint(point);
                break;
            }
            case TouchMessage::UP:
                break;
            default:
                break;
        }
    }
}
