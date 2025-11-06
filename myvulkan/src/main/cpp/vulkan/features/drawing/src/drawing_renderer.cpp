#include "drawing_renderer.hpp"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "pipeline_config.hpp"
#include "instance_buffer.hpp"
#include "touch_message.hpp"
#include "mesh.hpp"
#include "log.h"
#include "suraface_changed_message.hpp"

DrawingRenderer::DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surfaceContext) :
        mVkContext(vkContext), mSurfaceContext(std::move(surfaceContext)) {
    auto device = mVkContext.getDevice();

    mDescriptor = std::make_unique<Descriptor>(mVkContext.getDevice());
    mMesh = std::make_unique<Mesh>(vkContext);

    auto layout = mDescriptor->getLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);
    mPipeline = PipelineConfig::createPipeline(mVkContext, mPipelineLayout.get(), mSurfaceContext->getRenderPass());

    frameResources.resize(SurfaceContext::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; i++) {
        auto *frameResource = &frameResources[i];
        frameResource->instanceBuffer = std::make_unique<InstanceBuffer>(mVkContext);
        frameResource->uboBuffer = std::make_unique<UboBuffer<UboData>>(mVkContext);
        frameResource->descriptorSet = mDescriptor->allocate(frameResource->uboBuffer->getBuffer(), sizeof(UboData));
    }
}

DrawingRenderer::~DrawingRenderer() = default;

void DrawingRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

    auto *frameContext = mSurfaceContext->getFrameContext();
    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    auto cmdBuffer = frameContext->getCommandBuffer();
    auto *frameResource = &frameResources[currentFrameIndex];
    auto instanceBuffer = frameResource->instanceBuffer.get();
    auto uboBuffer = frameResource->uboBuffer.get();

    std::vector<InstanceData> instances;
    instances.reserve(mTouchPoints.size());
    for (const auto &mat: mTouchPoints) {
        instances.push_back(InstanceData{mat});
    }
    instanceBuffer->updateInstances(instances);
    uboBuffer->update({mProjection});

    mSurfaceContext->beginCommandBuffer(cmdBuffer);

    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                           mPipeline.get());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mPipelineLayout.get(),
            0,
            {frameResource->descriptorSet.get()},
            nullptr);

    vk::DeviceSize offsets[] = {0};
    vk::Buffer instanceBuffers[] = {instanceBuffer->getDeviceBuffer()};
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);

    mMesh->bind(cmdBuffer, 0);
    mMesh->draw(cmdBuffer, instanceBuffer->getInstanceCount());
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
