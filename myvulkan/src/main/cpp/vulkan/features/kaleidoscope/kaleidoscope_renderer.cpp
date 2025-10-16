#include "kaleidoscope_renderer.h"
#include "view_bounds.hpp"
#include "device_buffer.h"
#include "ubo_buffer.hpp"
#include "kaleidoscope_pipeline_config.hpp"

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
    mTexture = std::make_unique<Texture>(mVkContext, "textures/sample.png");
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

void KaleidoscopeRenderer::renderFrame() {
    mUbo->update({mProjectionMatrix});
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
