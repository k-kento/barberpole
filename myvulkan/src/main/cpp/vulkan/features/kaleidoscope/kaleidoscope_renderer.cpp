#include "kaleidoscope_renderer.h"
#include "view_bounds.hpp"
#include "vulkan_buffer.h"
#include "descriptor.hpp"
#include "ubo_buffer.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "log.h"
#include "vertex.hpp"
#include "mirror_tile_grid.hpp"
#include "kaleidoscope_pipeline_config.hpp"

KaleidoscopeRenderer::KaleidoscopeRenderer(VulkanContext *vkContext,
                                           ANativeWindow *window,
                                           RenderPass *renderPass)
        : mVkContext(vkContext), mRenderPass(renderPass) {
    auto device = mVkContext->getVkDevice();

    auto windowWidth = ANativeWindow_getWidth(window);
    auto windowHeight = ANativeWindow_getHeight(window);

    mRegularPolygonMesh = std::make_unique<RegularPolygon>(3, 1.0f);

    auto viewBounds = ViewBounds::fromSize(windowWidth, windowHeight);
    mProjectionMatrix = viewBounds.toOrthoMatrix();

    auto slices = MirrorTileGrid::createTileGrid(&viewBounds);

    for (auto &slice: slices) {
        instanceData.push_back({slice});
    }

    createBuffer();
    createDescriptorSet();
    mUboWrapper.emplace(mUboBuffer->getMemory());

    vk::PipelineLayoutCreateInfo layoutInfo{};
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);
    mPipeline = KaleidoscopePipelineConfig::createPipeline(*mVkContext, mPipelineLayout.get(), *mRenderPass);
}

void KaleidoscopeRenderer::renderFrame() {
    mUboData.projection = mProjectionMatrix;
    mUboWrapper->update(mVkContext->getVkDevice(), mUboData);
}

void KaleidoscopeRenderer::recordDrawCommands(vk::CommandBuffer cmdBuffer) {
    // パイプラインバインド
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());

    // ディスクリプタセットバインド
    std::array<vk::DescriptorSet, 1> sets = {mDescriptorSet};
    cmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout.get(), 0, sets, nullptr);

    // 頂点バッファとインスタンスバッファ
    vk::Buffer vertexBuffers[] = {mVertexBuffer->getBuffer()};
    vk::Buffer instanceBuffers[] = {mInstanceBuffer->getBuffer()};
    vk::DeviceSize offsets[] = {0};

    cmdBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);

    // インデックスバッファ
    cmdBuffer.bindIndexBuffer(mIndexBuffer->getBuffer(), 0, vk::IndexType::eUint16);

    auto indexCount = static_cast<uint32_t>(mRegularPolygonMesh->indices.size());
    uint32_t instanceCount = instanceData.size();
    cmdBuffer.drawIndexed(indexCount, instanceCount, 0, 0, 0);
}

void KaleidoscopeRenderer::createBuffer() {
    /* Vertex Buffer */
    vk::Device device = mVkContext->getVkDevice();
    vk::PhysicalDevice physicalDevice = mVkContext->getVkPhysicalDevice();

    VkDeviceSize bufferSize = sizeof(mRegularPolygonMesh->vertices[0]) * mRegularPolygonMesh->vertices.size();

    mVertexBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            bufferSize,
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    mVertexBuffer->copyFrom(mRegularPolygonMesh->vertices.data(), bufferSize);

    /* Index Buffer */
    bufferSize = sizeof(mRegularPolygonMesh->indices[0]) * mRegularPolygonMesh->indices.size();

    mIndexBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            bufferSize,
            vk::BufferUsageFlagBits::eIndexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    mIndexBuffer->copyFrom(mRegularPolygonMesh->indices.data(), bufferSize);

    /* Instance Buffer */
    uint32_t instanceCount = instanceData.size();
    VkDeviceSize instanceBufferSize = sizeof(InstanceData) * instanceCount;

    mInstanceBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            instanceBufferSize,
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    mInstanceBuffer->copyFrom(instanceData.data(), instanceBufferSize);

    /* UBO Buffer */
    mUboBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            sizeof(UBO),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );
}

void KaleidoscopeRenderer::createDescriptorSet() {
    vk::Device device = mVkContext->getVkDevice();

    Descriptor::Config config{};
    config.binding = 0;
    config.stageFlags = vk::ShaderStageFlagBits::eVertex;
    config.maxSets = 1;

    mDescriptorManager = std::make_unique<Descriptor>(device, config);
    mDescriptorSet = mDescriptorManager->allocateDescriptorSet(device, mUboBuffer->getBuffer(), sizeof(UBO));
}
