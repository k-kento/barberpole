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

    mMesh = std::make_unique<KaleidoscopeMesh>(mVkContext);
    mInstanceData = std::make_unique<KaleidoscopeInstanceBuffer>(mVkContext, viewBounds);
    mUbo = std::make_unique<KaleidoscopeUbo>(mVkContext);

    auto layout = mUbo->getDescriptorSetLayout();
    vk::DescriptorSetLayout layouts[] = {layout};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    mPipeline = KaleidoscopePipelineConfig::createPipeline(mVkContext, mPipelineLayout.get(), mRenderPass);
}

void KaleidoscopeRenderer::renderFrame() {
    mUbo->update({mProjectionMatrix});
}

void KaleidoscopeRenderer::recordDrawCommands(vk::CommandBuffer cmdBuffer) {
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

    std::array<vk::DescriptorSet, 1> descriptorSets = {mUbo->getDescriptorSet()};
    cmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout.get(), 0, descriptorSets, nullptr);

    vk::Buffer vertexBuffers[] = {mMesh->getVertexBuffer()};
    vk::Buffer instanceBuffers[] = {mInstanceData->getDeviceBuffer()};
    vk::DeviceSize offsets[] = {0};

    cmdBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);
    cmdBuffer.bindIndexBuffer(mMesh->getIndexBuffer(), 0, vk::IndexType::eUint16);

    cmdBuffer.drawIndexed(mMesh->getIndexCount(), mInstanceData->getInstanceCount(), 0, 0, 0);
}
