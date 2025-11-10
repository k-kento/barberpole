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
#include "renderer_constants.hpp"
#include "compute_params.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surfaceContext) :
        mVkContext(vkContext),
        mSurfaceContext(std::move(surfaceContext)) {
    auto device = mVkContext.getDevice();

    // mInputBuffer = std::make_unique<InputBuffer>(vkContext);
    mStroke = std::make_unique<Stroke>();

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

    beginFrame(frameContext, cmdBuffer);

//    recordComputePass(cmdBuffer, frameContext, numPoints);
//    insertComputeToGraphicsBarrier(cmdBuffer);
    recordGraphicsPass(cmdBuffer, frameContext, 0);
    endFrame(cmdBuffer, frameContext);
}

FrameContext &DrawingRenderer::getCurrentFrameContext() {
    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    return *mFrameContexts[currentFrameIndex];
}

void DrawingRenderer::beginFrame(FrameContext &frameContext, vk::CommandBuffer cmdBuffer) {
    updateUniforms(frameContext);

    if (!frameContext.initialized) {
        updateComputeDescriptor(frameContext);
        frameContext.initialized = true;
    }

    mSurfaceContext->beginCommandBuffer(cmdBuffer);
}

void DrawingRenderer::updateUniforms(FrameContext &frameContext) {
    auto uboBuffer = frameContext.getUboBuffer();
    uboBuffer->update({mProjection});
}

void DrawingRenderer::updateComputeDescriptor(FrameContext &frameContext) {
//    mComputeDescriptor->updateStorageBuffers(
//            frameContext.getComputeDescriptorSet(),
//            mInputBuffer->getBuffer(),
//            sizeof(InputVertex) * MAX_INPUT_POINTS,
//            mComputeBuffer->getBuffer(),
//            sizeof(InputVertex) * MAX_COMPUTE_OUTPUT_VERTICES);
}

//uint32_t DrawingRenderer::uploadInputVertices() {
//    const uint32_t cpuCount = static_cast<uint32_t>(mTouchPoints.size());
//    if (cpuCount == 0) return 0;
//
//    // 今回 GPU に必要な範囲を決定
//    uint32_t start = (mPrevPointCount > 0) ? (mPrevPointCount - 1) : 0;
//    if (start > cpuCount) start = 0; // 念のため
//    uint32_t count = cpuCount - start;
//    if (count == 0) return cpuCount;
//
//    // deque → 連続領域へ（該当範囲のみ）
////    mInputVertices.clear();
////    mInputVertices.reserve(count);
////    auto itStart = mTouchPoints.begin() + start;
////    mInputVertices.insert(mInputVertices.end(), itStart, mTouchPoints.end());
//
//    // GPU バッファを部分更新（dst は start から）
//    // mInputBuffer->update(mInputVertices.data(), count, start);
//
//    return cpuCount; // 全体点数（描画・ディスパッチの基準）
//}

//void DrawingRenderer::recordComputePass(vk::CommandBuffer cmdBuffer,
//                                        FrameContext &frameContext,
//                                        uint32_t numPoints) {
//    if (numPoints <= 1 || numPoints <= mPrevPointCount) return;
//
//    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mComputePipeline->getPipeline());
//    cmdBuffer.bindDescriptorSets(
//            vk::PipelineBindPoint::eCompute,
//            mComputePipeline->getLayout(),
//            0,
//            frameContext.getComputeDescriptorSet(),
//            {});
//
//    uint32_t startIndex = mPrevPointCount > 0 ? mPrevPointCount - 1 : 0;
//    uint32_t newCount = numPoints - startIndex;
//
//    const uint32_t numGroups = (newCount + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
//    ComputeParams params{startIndex, newCount};
//    cmdBuffer.pushConstants(mComputePipeline->getLayout(),
//                            vk::ShaderStageFlagBits::eCompute,
//                            0,
//                            sizeof(ComputeParams),
//                            &params);
//    cmdBuffer.dispatch(numGroups, 1, 1);
//
//    mPrevPointCount = numPoints;
//}

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

    auto *vertexBuffer = frameContext.getVertexBuffer();

    vk::DeviceSize offsets[] = {0};
    cmdBuffer.bindVertexBuffers(0, vertexBuffer->getBuffer(), offsets);

    // 差分更新
    auto totalVertexCount = mStroke->getVertexCount();
    auto writtenVertexCount = vertexBuffer->getWrittenVertexCount();
    if (writtenVertexCount < totalVertexCount) {
        uint32_t newCount = totalVertexCount - writtenVertexCount;
        auto &vertices = mStroke->getVertices();
        const InputVertex *newVertices = vertices.data() + writtenVertexCount;
        vertexBuffer->update(newVertices, newCount, writtenVertexCount);
        // cmdBuffer.draw(newCount, 1, writtenVertexCount, 0);
    }

    if (totalVertexCount > 0) {
        cmdBuffer.draw(totalVertexCount, 1, 0, 0);
    }

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
                auto point = InputVertex{glm::vec4{normalizedX, normalizedY, 0.0f, 0.0f}};
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
