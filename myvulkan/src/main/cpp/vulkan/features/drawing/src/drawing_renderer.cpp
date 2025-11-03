#include "drawing_renderer.hpp"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "pipeline_config.hpp"
#include "ubo.hpp"
#include "engine_config.hpp"
#include "instance_buffer.hpp"
#include "touch_message.hpp"
#include "mesh.hpp"
#include "log.h"
#include "suraface_changed_message.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext,
                                 RenderPass &renderPass) :
        mVkContext(vkContext),
        mRenderPass(renderPass) {

    auto device = mVkContext.getDevice();

    mUbo = std::make_unique<Ubo>(mVkContext, EngineConfig::MAX_FRAMES_IN_FLIGHT);
    mInstanceBuffer = std::make_unique<InstanceBuffer>(mVkContext);
    mMesh = std::make_unique<Mesh>(vkContext);

    auto layout = mUbo->getDescriptorSetLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    mPipeline = PipelineConfig::createPipeline(mVkContext, mPipelineLayout.get(), mRenderPass);
}

DrawingRenderer::~DrawingRenderer() = default;

void DrawingRenderer::renderFrame(float deltaTimeMs, uint32_t currentFrame) {
    if (!mTouchPoints.empty()) {

    }
    std::vector<InstanceData> instances;
    instances.reserve(mTouchPoints.size());
    for (const auto &mat: mTouchPoints) {
        instances.push_back(InstanceData{mat});
    }
    mInstanceBuffer->updateInstances(currentFrame, instances);
    mUbo->update({mProjection}, currentFrame);
}

void DrawingRenderer::recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t frameIndex) {
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mPipelineLayout.get(),
            0,
            {mUbo->getDescriptorSet(frameIndex)},
            nullptr);

    vk::DeviceSize offsets[] = {0};
    vk::Buffer instanceBuffers[] = {mInstanceBuffer->getDeviceBuffer(frameIndex)};
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);

    mMesh->bind(cmdBuffer, 0);
    mMesh->draw(cmdBuffer, mInstanceBuffer->getInstanceCount(frameIndex));
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
