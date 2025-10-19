#include "kaleidoscope_renderer.h"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "kaleidoscope_pipeline_config.hpp"
#include "rotation_message.hpp"

KaleidoscopeRenderer::KaleidoscopeRenderer(VulkanContext &vkContext,
                                           RenderPass &renderPass,
                                           uint32_t windowWidth,
                                           uint32_t windowHeight)
        : mVkContext(vkContext), mRenderPass(renderPass) {

    auto device = mVkContext.getVkDevice();

    auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
    mProjectionMatrix = viewBounds.toOrthoMatrix();

    mMeshManager = std::make_unique<KaleidoscopeMeshManager>(mVkContext);
    mInstanceData = std::make_unique<KaleidoscopeInstanceBuffer>(mVkContext, viewBounds);
    mTexture = std::make_unique<Texture>(mVkContext, "images/bus_main .png");
    mUbo = std::make_unique<KaleidoscopeUbo>(mVkContext, *mTexture);

    auto layout = mUbo->getDescriptorSetLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    mPipeline = KaleidoscopePipelineConfig::createPipeline(mVkContext, mPipelineLayout.get(), mRenderPass);

    // Descriptor 用に登録（UBO + Sampler）
    vk::DescriptorImageInfo imageInfo{
            mTexture->getSampler(),
            mTexture->getImageView(),
            vk::ImageLayout::eShaderReadOnlyOptimal,
    };

    vk::WriteDescriptorSet write(
            mUbo->getDescriptorSet(),
            1,
            0,
            1,
            vk::DescriptorType::eCombinedImageSampler,
            &imageInfo
    );

    device.updateDescriptorSets(1, &write, 0, nullptr);
}

void KaleidoscopeRenderer::renderFrame(float deltaTimeMs) {
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

    mUbo->update({mProjectionMatrix, mUvMatrix});
}

void KaleidoscopeRenderer::recordDrawCommands(vk::CommandBuffer cmdBuffer) {
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

    std::array<vk::DescriptorSet, 1> descriptorSets = {mUbo->getDescriptorSet()};
    cmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout.get(), 0, descriptorSets, nullptr);

    vk::Buffer instanceBuffers[] = {mInstanceData->getDeviceBuffer()};
    vk::DeviceSize offsets[] = {0};

    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);
    mMeshManager->bind(cmdBuffer, 0);
    mMeshManager->draw(cmdBuffer, mInstanceData->getInstanceCount());
}

void KaleidoscopeRenderer:: handleMessage(std::unique_ptr<RenderMessage> message) {
    if (const auto* rotationMsg = dynamic_cast<const RotationMessage*>(message.get())) {
        mRotationState = rotationMsg->rotationState;
    }
}
