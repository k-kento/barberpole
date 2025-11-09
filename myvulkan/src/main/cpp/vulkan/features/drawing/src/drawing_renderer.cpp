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

    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    auto frameContext = mFrameContexts[currentFrameIndex].get();
    auto cmdBuffer = frameContext->getCommandBuffer();
    auto uboBuffer = frameContext->getUboBuffer();
    auto isFirst = frameContext->isFirst;

    uboBuffer->update({mProjection});

    if (isFirst) {
        mComputeDescriptor->updateStorageBuffers(
                frameContext->getComputeDescriptorSet(),
                mInputBuffer->getBuffer(),
                sizeof(InputVertex) * mTouchPoints.size(),
                mComputeBuffer->getBuffer(),
                sizeof(InputVertex) * 100
        );
    }

    mSurfaceContext->beginCommandBuffer(cmdBuffer);

    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mComputePipeline->getPipeline());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eCompute,
            mComputePipeline->getLayout(),
            0,
            frameContext->getComputeDescriptorSet(),
            {}
    );

    constexpr uint32_t WORKGROUP_SIZE = 64;

    std::vector<InputVertex> points;
    points.reserve(mTouchPoints.size());
    for (const auto &mat: mTouchPoints) {
        points.push_back({mat});
    }
    mInputBuffer->update({points});
    uint32_t numPoints = static_cast<uint32_t>(points.size());
    uint32_t numGroups = (numPoints + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
    if (numPoints > 0) {
        cmdBuffer.pushConstants(mComputePipeline->getLayout(),
                                vk::ShaderStageFlagBits::eCompute,
                                0,
                                sizeof(uint32_t),
                                &numPoints);
        cmdBuffer.dispatch(numGroups, 1, 1);
    }

    // ======== バリア (Compute → Graphics) ========
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
            nullptr,  // memory barriers
            barrier,  // buffer barrier
            nullptr   // image barriers
    );

    mSurfaceContext->beginRenderPass(cmdBuffer);

    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicPipeline->getPipeline());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mGraphicPipeline->getLayout(),
            0,
            {frameContext->getGraphicDescriptorSet()},
            nullptr);

    vk::DeviceSize offsets[] = {0};
    cmdBuffer.bindVertexBuffers(0, mComputeBuffer->getBuffer(), offsets);

    const uint32_t segs = (numPoints > 0) ? (numPoints - 1) : 0; // TODO
    const uint32_t drawVerts = segs * 6; // TODO
    if (drawVerts > 0) {
        cmdBuffer.draw(drawVerts, 1, 0, 0);
    }

    mSurfaceContext->endRenderPass(cmdBuffer);
    mSurfaceContext->endCommandBuffer(cmdBuffer);

    mSurfaceContext->submit(cmdBuffer);
    mSurfaceContext->present();

    frameContext->isFirst = false;
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
