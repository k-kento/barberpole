#include "kaleidoscope_renderer.hpp"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "kaleidoscope_pipeline_config.hpp"
#include "rotation_message.hpp"
#include "update_texture_message.hpp"
#include "surface_context.hpp"

KaleidoscopeRenderer::KaleidoscopeRenderer(VulkanContext &vkContext,
                                           uint32_t windowWidth,
                                           uint32_t windowHeight,
                                           uint32_t deviceRotationDegree,
                                           const std::string &texturePath,
                                           std::unique_ptr<SurfaceContext> surfaceContext)
        : mVkContext(vkContext), mSurfaceContext(std::move(surfaceContext)) {

    auto device = mVkContext.getDevice();

    auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
    float deviceRotationRad = glm::radians(static_cast<float>(deviceRotationDegree));
    auto deviceRotation = glm::rotate(glm::mat4(1.0f), deviceRotationRad, glm::vec3(0.0f, 0.0f, 1.0f));
    mProjectionMatrix = deviceRotation * viewBounds.toOrthoMatrix();

    mMeshManager = std::make_unique<KaleidoscopeMeshManager>(mVkContext);
    mInstanceData = std::make_unique<KaleidoscopeInstanceBuffer>(mVkContext, viewBounds);
    mTexture = std::make_unique<Texture>(mVkContext, texturePath);
    mUbo = std::make_unique<KaleidoscopeUbo>(mVkContext, *mTexture, SurfaceContext::MAX_FRAMES_IN_FLIGHT);

    auto layout = mUbo->getDescriptorSetLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    mPipeline = KaleidoscopePipelineConfig{}.createPipeline(mVkContext, mPipelineLayout.get(), mSurfaceContext->getRenderPass());
}

void KaleidoscopeRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();

    auto rotationState = mRotationState;
    if (rotationState != RotationState::None) {

        mUvAngle += (rotationState == RotationState::RotatingCCW ? -1.0f : 1.0f) * rotationSpeed * deltaTimeMs;
        mUvAngle = glm::mod(mUvAngle, glm::two_pi<float>()); // 0 ~ 2π に収める

        // UV を 0.5, 0.5 を中心に回転
        glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), mUvAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

        mUvMatrix = translateBack * rotation * translateToCenter;
    }

    mUbo->update({mProjectionMatrix, mUvMatrix}, currentFrameIndex);

    mSurfaceContext->recordCommandBuffers([this]() {
        auto *frameContext = mSurfaceContext->getFrameContext();
        auto cmdBuffer = frameContext->getCommandBuffer();
        auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();

        cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

        cmdBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                mPipelineLayout.get(),
                0,
                {mUbo->getDescriptorSet(currentFrameIndex)},
                nullptr);

        vk::Buffer instanceBuffers[] = {mInstanceData->getDeviceBuffer()};
        vk::DeviceSize offsets[] = {0};

        cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);
        mMeshManager->bind(cmdBuffer, 0);
        mMeshManager->draw(cmdBuffer, mInstanceData->getInstanceCount());
    });

    mSurfaceContext->submit();
    mSurfaceContext->present();
}

void KaleidoscopeRenderer::handleMessage(std::unique_ptr<RenderMessage> message) {
    if (auto rotationMsg = dynamic_cast<const RotationMessage *>(message.get())) {
        mRotationState = rotationMsg->rotationState;
    } else if (auto updateTextureMsg = dynamic_cast<const UpdateTextureMessage *>(message.get())) {
        updateTexture(updateTextureMsg->filePath);
    }
}

void KaleidoscopeRenderer::updateTexture(const std::string &path) {
    mTexture = std::make_unique<Texture>(mVkContext, path);
    mUbo->setTexture(*mTexture);
    // TODO 再 bindDescriptorSets
}
