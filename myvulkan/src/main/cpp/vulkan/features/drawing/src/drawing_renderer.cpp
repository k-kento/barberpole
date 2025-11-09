#include "drawing_renderer.hpp"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "graphic_pipeline.hpp"
#include "touch_message.hpp"
#include "log.h"
#include "suraface_changed_message.hpp"
#include "frame_context.hpp"
#include "input_vertex.hpp"
#include "ubo_data.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surfaceContext) :
        mVkContext(vkContext),
        mSurfaceContext(std::move(surfaceContext)) {
    auto device = mVkContext.getDevice();

    mInputBuffer = std::make_unique<InputBuffer>(vkContext);

    mGraphicDescriptor = std::make_unique<GraphicDescriptor>(mVkContext.getDevice());
    mGraphicPipeline = std::make_unique<GraphicPipeline>(mVkContext,
                                                         mSurfaceContext->getRenderPass(),
                                                         *mGraphicDescriptor);

    mComputeDescriptor = std::make_unique<ComputeDescriptor>(device);
    mComputePipeline = std::make_unique<ComputePipeline>(vkContext, *mComputeDescriptor);
    mComputeBuffer = std::make_unique<ComputeBuffer>(mVkContext);

    mFrameContexts.reserve(SurfaceContext::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
        mFrameContexts.emplace_back(
                std::make_unique<FrameContext>(
                        vkContext,
                        *mGraphicDescriptor,
                        *mComputeDescriptor));
    }
}

DrawingRenderer::~DrawingRenderer() = default;

void DrawingRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

    auto &frameContext = getCurrentFrameContext();
    auto cmdBuffer = frameContext.getCommandBuffer();

    prepareFrame(frameContext, cmdBuffer);

    const uint32_t numPoints = uploadInputVertices();
    recordComputePass(cmdBuffer, frameContext, numPoints);
    insertComputeToGraphicsBarrier(cmdBuffer);
    recordGraphicsPass(cmdBuffer, frameContext, numPoints);

    finalizeFrame(cmdBuffer, frameContext);
}

FrameContext &DrawingRenderer::getCurrentFrameContext() {
    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    return *mFrameContexts[currentFrameIndex];
}

void DrawingRenderer::prepareFrame(FrameContext &frameContext, vk::CommandBuffer cmdBuffer) {
    updateUniforms(frameContext);

    if (frameContext.isFirst) {
        updateComputeDescriptor(frameContext);
    }

    mSurfaceContext->beginCommandBuffer(cmdBuffer);
}

void DrawingRenderer::updateUniforms(FrameContext &frameContext) {
    auto uboBuffer = frameContext.getUboBuffer();
    uboBuffer->update({mProjection});
}

void DrawingRenderer::updateComputeDescriptor(FrameContext &frameContext) {
    mComputeDescriptor->updateStorageBuffers(
            frameContext.getComputeDescriptorSet(),
            mInputBuffer->getBuffer(),
            sizeof(InputVertex) * mTouchPoints.size(),
            mComputeBuffer->getBuffer(),
            sizeof(InputVertex) * 100);
}

uint32_t DrawingRenderer::uploadInputVertices() {
    mInputVertices.assign(mTouchPoints.begin(), mTouchPoints.end());
    mInputBuffer->update({mInputVertices});
    return static_cast<uint32_t>(mInputVertices.size());
}

void DrawingRenderer::recordComputePass(vk::CommandBuffer cmdBuffer,
                                        FrameContext &frameContext,
                                        uint32_t numPoints) {
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mComputePipeline->getPipeline());
    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eCompute,
            mComputePipeline->getLayout(),
            0,
            frameContext.getComputeDescriptorSet(),
            {});

    if (numPoints == 0) {
        return;
    }

    constexpr uint32_t WORKGROUP_SIZE = 64;
    const uint32_t numGroups = (numPoints + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;

    cmdBuffer.pushConstants(mComputePipeline->getLayout(),
                            vk::ShaderStageFlagBits::eCompute,
                            0,
                            sizeof(uint32_t),
                            &numPoints);
    cmdBuffer.dispatch(numGroups, 1, 1);
}

void DrawingRenderer::insertComputeToGraphicsBarrier(vk::CommandBuffer cmdBuffer) {
    vk::BufferMemoryBarrier barrier{};
    barrier.srcAccessMask = vk::AccessFlagBits::eShaderWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eVertexAttributeRead;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.buffer = mComputeBuffer->getBuffer();
    barrier.offset = 0;
    barrier.size = VK_WHOLE_SIZE;

    cmdBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eVertexInput,
            {},
            nullptr,
            barrier,
            nullptr);
}

void DrawingRenderer::recordGraphicsPass(vk::CommandBuffer cmdBuffer,
                                         FrameContext &frameContext,
                                         uint32_t numPoints) {
    mSurfaceContext->beginRenderPass(cmdBuffer);

    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicPipeline->getPipeline());
    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mGraphicPipeline->getLayout(),
            0,
            {frameContext.getGraphicDescriptorSet()},
            nullptr);

    vk::DeviceSize offsets[] = {0};
    cmdBuffer.bindVertexBuffers(0, mComputeBuffer->getBuffer(), offsets);

    const uint32_t drawVerts = calculateDrawVertices(numPoints);
    if (drawVerts > 0) {
        cmdBuffer.draw(drawVerts, 1, 0, 0);
    }

    mSurfaceContext->endRenderPass(cmdBuffer);
}

uint32_t DrawingRenderer::calculateDrawVertices(uint32_t numPoints) const {
    if (numPoints < 2) {
        return 0;
    }

    const uint32_t segments = numPoints - 1;
    return segments * 6;
}

void DrawingRenderer::finalizeFrame(vk::CommandBuffer cmdBuffer, FrameContext &frameContext) {
    mSurfaceContext->endCommandBuffer(cmdBuffer);
    mSurfaceContext->submit(cmdBuffer);
    mSurfaceContext->present();

    frameContext.isFirst = false;
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
                mTouchPoints.push_back(InputVertex{glm::vec4{normalizedX, normalizedY, 0.0f, 0.0f}});
                break;
            }
            case TouchMessage::UP:
                mTouchPoints.clear();
                break;
            default:
                break;
        }
    }
}
