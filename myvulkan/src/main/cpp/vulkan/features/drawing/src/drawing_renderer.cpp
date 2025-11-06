#include "drawing_renderer.hpp"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "pipeline_config.hpp"
#include "ubo.hpp"
#include "instance_buffer.hpp"
#include "touch_message.hpp"
#include "mesh.hpp"
#include "log.h"
#include "suraface_changed_message.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surfaceContext) :
        mVkContext(vkContext), mSurfaceContext(std::move(surfaceContext)) {
    auto device = mVkContext.getDevice();

    mUbo = std::make_unique<Ubo>(mVkContext, SurfaceContext::MAX_FRAMES_IN_FLIGHT);
    mInstanceBuffer = std::make_unique<InstanceBuffer>(mVkContext);
    mMesh = std::make_unique<Mesh>(vkContext);

    auto layout = mUbo->getDescriptorSetLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);
    mPipeline = PipelineConfig::createPipeline(mVkContext, mPipelineLayout.get(), mSurfaceContext->getRenderPass());
}

DrawingRenderer::~DrawingRenderer() = default;

void DrawingRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

    auto *frameContext = mSurfaceContext->getFrameContext();
    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    auto cmdBuffer = frameContext->getCommandBuffer();

    std::vector<InstanceData> instances;
    instances.reserve(mTouchPoints.size());
    for (const auto &mat: mTouchPoints) {
        instances.push_back(InstanceData{mat});
    }
    mInstanceBuffer->updateInstances(currentFrameIndex, instances);
    mUbo->update({mProjection}, currentFrameIndex);

    mSurfaceContext->beginCommandBuffer(cmdBuffer);

    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                           mPipeline.get());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mPipelineLayout.get(),
            0,
            {mUbo->getDescriptorSet(currentFrameIndex)},
            nullptr);

    vk::DeviceSize offsets[] = {0};
    vk::Buffer instanceBuffers[] = {mInstanceBuffer->getDeviceBuffer(currentFrameIndex)};
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);

    mMesh->bind(cmdBuffer, 0);
    mMesh->draw(cmdBuffer, mInstanceBuffer->getInstanceCount(currentFrameIndex));
    mSurfaceContext->endCommandBuffer(cmdBuffer);

    mSurfaceContext->submit();
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
                auto translate = glm::translate(glm::mat4(1.0f), glm::vec3(normalizedX, normalizedY, 0.0f));
                auto scale = glm::scale(glm::mat4(1.0f), {0.2f, 0.2f, 1.0});
                mTouchPoints.push_back(translate * scale);
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
