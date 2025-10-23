#include "kaleidoscope_renderer.h"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "kaleidoscope_pipeline_config.hpp"
#include "rotation_message.hpp"
#include "update_texture_message.hpp"

KaleidoscopeRenderer::KaleidoscopeRenderer(VulkanContext &vkContext,
                                           RenderPass &renderPass,
                                           uint32_t windowWidth,
                                           uint32_t windowHeight,
                                           uint32_t deviceRotationDegree,
                                           const std::string &texturePath)
        : mVkContext(vkContext), mRenderPass(renderPass) {

    auto device = mVkContext.getDevice();

    auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
    float deviceRotationRad = glm::radians(static_cast<float>(deviceRotationDegree));
    auto deviceRotation = glm::rotate(glm::mat4(1.0f), deviceRotationRad, glm::vec3(0.0f, 0.0f, 1.0f));
    mProjectionMatrix = deviceRotation * viewBounds.toOrthoMatrix();

    mMeshManager = std::make_unique<KaleidoscopeMeshManager>(mVkContext);
    mInstanceData = std::make_unique<KaleidoscopeInstanceBuffer>(mVkContext, viewBounds);
    mTexture = std::make_unique<Texture>(mVkContext, texturePath);
    mUbo = std::make_unique<KaleidoscopeUbo>(mVkContext, *mTexture, MAX_FRAMES_IN_FLIGHT);

    auto layout = mUbo->getDescriptorSetLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    mPipeline = KaleidoscopePipelineConfig::createPipeline(mVkContext, mPipelineLayout.get(), mRenderPass);
}

void KaleidoscopeRenderer::renderFrame(float deltaTimeMs, uint32_t currentFrame) {
    auto rotationState = mRotationState;
    if (rotationState != RotationState::None) {

        mUvAngle += (rotationState == RotationState::RotatingCCW ? -1 : 1) * rotationSpeed * deltaTimeMs;
        mUvAngle = glm::mod(mUvAngle, glm::two_pi<float>()); // 0 ~ 2π に収める

        // UV を 0.5, 0.5 を中心に回転
        glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), mUvAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

        mUvMatrix = translateBack * rotation * translateToCenter;
    }

    mUbo->update({mProjectionMatrix, mUvMatrix}, currentFrame);
}

void KaleidoscopeRenderer::recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t frameIndex) {
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mPipelineLayout.get(),
            0,
            {mUbo->getDescriptorSet(frameIndex)},
            nullptr);

    vk::Buffer instanceBuffers[] = {mInstanceData->getDeviceBuffer()};
    vk::DeviceSize offsets[] = {0};

    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);
    mMeshManager->bind(cmdBuffer, 0);
    mMeshManager->draw(cmdBuffer, mInstanceData->getInstanceCount());
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
