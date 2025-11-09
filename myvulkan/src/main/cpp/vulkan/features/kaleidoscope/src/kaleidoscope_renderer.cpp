#include "kaleidoscope_renderer.hpp"
#include "view_bounds.hpp"
#include "kaleidoscope_pipeline_config.hpp"
#include "rotation_message.hpp"
#include "update_texture_message.hpp"
#include "surface_context.hpp"
#include "kaleidoscope_frame_context.hpp"
#include "regular_polygon.hpp"

KaleidoscopeRenderer::KaleidoscopeRenderer(VulkanContext &vkContext,
                                           uint32_t windowWidth,
                                           uint32_t windowHeight,
                                           uint32_t deviceRotationDegree,
                                           const std::string &texturePath,
                                           std::unique_ptr<SurfaceContext> surfaceContext)
        : mVkContext(vkContext), mSurfaceContext(std::move(surfaceContext)) {

    auto device = mVkContext.getDevice();

    mDescriptor = std::make_unique<KaleidoscopeDescriptor>(device);

    auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
    float deviceRotationRad = glm::radians(static_cast<float>(deviceRotationDegree));
    auto deviceRotation = glm::rotate(glm::mat4(1.0f), deviceRotationRad, glm::vec3(0.0f, 0.0f, 1.0f));
    mProjectionMatrix = deviceRotation * viewBounds.toOrthoMatrix();

    auto polygon = RegularPolygon{3, 1.0f};

    vk::DescriptorSetLayout layouts[] = {mDescriptor->getLayout()};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    mPipeline = KaleidoscopePipelineConfig{}.createPipeline(mVkContext,
                                                            mPipelineLayout.get(),
                                                            mSurfaceContext->getRenderPass());

    mFrameContexts.reserve(SurfaceContext::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
        mFrameContexts.emplace_back(std::make_unique<KaleidoscopeFrameContext>(vkContext, viewBounds));
    }

    mMesh = std::make_unique<MeshBuffer<Vertex>>(vkContext, polygon.vertices, polygon.indices);
    updateTexture(texturePath);
}

void KaleidoscopeRenderer::renderFrame(float deltaTimeMs) {
    mSurfaceContext->acquireNextImage();

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

    auto currentFrameIndex = mSurfaceContext->getCurrentFrameIndex();
    auto frameContext = mFrameContexts[currentFrameIndex].get();

    frameContext->getUniformBuffer().update({mProjectionMatrix, mUvMatrix});


    if (frameContext->isTextureUpdated) {
        frameContext->isTextureUpdated = false;
        if (frameContext->hasDescriptorSet()) {
            mDescriptor->update(
                    frameContext->getDescriptorSet(),
                    frameContext->getUniformBuffer().getBuffer(),
                    sizeof(KaleidoscopeFrameContext::UboData),
                    mTexture->getImageView(),
                    mTexture->getSampler());
        } else {
            auto descriptorSet = mDescriptor->allocate(
                    frameContext->getUniformBuffer().getBuffer(),
                    sizeof(KaleidoscopeFrameContext::UboData),
                    mTexture->getImageView(),
                    mTexture->getSampler());
            frameContext->updateDescriptorSet(std::move(descriptorSet));
        }
    }
    recordCommandBuffer(frameContext);

    mSurfaceContext->submit(frameContext->getCmdBuffer());
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
    for (uint32_t i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
        mFrameContexts[i]->isTextureUpdated = true;
    }
}

void KaleidoscopeRenderer::recordCommandBuffer(KaleidoscopeFrameContext* frameContext) {
    auto cmdBuffer = frameContext->getCmdBuffer();
    auto &uniformBuffer = frameContext->getUniformBuffer();
    auto &instanceBuffer = frameContext->getInstanceBuffer();
    mSurfaceContext->beginCommandBuffer(cmdBuffer);

    mSurfaceContext->beginRenderPass(cmdBuffer);
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mPipelineLayout.get(),
            0,
            {frameContext->getDescriptorSet()},
            nullptr);

    vk::Buffer instanceBuffers[] = {frameContext->getInstanceBuffer().getDeviceBuffer()};

    VkDeviceSize offsets[] = {0};
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);

    mMesh->bind(cmdBuffer, 0);
    mMesh->draw(cmdBuffer, instanceBuffer.getInstanceCount());

    mSurfaceContext->endRenderPass(cmdBuffer);
    mSurfaceContext->endCommandBuffer(cmdBuffer);
}
