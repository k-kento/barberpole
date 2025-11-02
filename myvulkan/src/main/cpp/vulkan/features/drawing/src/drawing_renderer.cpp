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

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext,
                                 RenderPass &renderPass,
                                 ViewBounds &viewBounds,
                                 glm::mat4 projectionMatrix) :
        mVkContext(vkContext),
        mRenderPass(renderPass),
        mViewBounds(viewBounds),
        mProjectionMatrix(projectionMatrix) {

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
    for (const auto &mat : mTouchPoints) {
        instances.push_back(InstanceData{mat});
    }
    mInstanceBuffer->updateInstances(currentFrame, instances);
    mUbo->update({mProjectionMatrix}, currentFrame);
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
    if (auto touchMsg = dynamic_cast<const TouchMessage *>(message.get())) {
        switch (touchMsg->touchType) {
            case TouchMessage::Move: {
                auto tapX = touchMsg->x;
                auto tapY = touchMsg->y;
                auto point = glm::translate(glm::mat4(1.0f), glm::vec3(tapX, tapY, 0.0f));
                mTouchPoints.push_back(point);
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


