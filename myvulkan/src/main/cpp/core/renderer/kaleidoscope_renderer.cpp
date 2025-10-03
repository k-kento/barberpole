#include "kaleidoscope_renderer.h"
#include "vulkan_utils.h"
#include "graphics_pipeline.h"
#include "memory/vulkan_buffer.h"
#include "descriptor.hpp"
#include "memory/ubo_buffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "kaleidoscope/kaleidoscope_pipeline_config.hpp"
#include "log.h"

KaleidoscopeRenderer::KaleidoscopeRenderer(VulkanContext *vkContext, ANativeWindow *window) : VulkanRenderer(vkContext,
                                                                                                             window) {

    auto windowWidth = ANativeWindow_getWidth(window);
    auto windowHeight = ANativeWindow_getHeight(window);

    updateProjectionMatrix(windowWidth, windowHeight);
    createBuffer();
    createDescriptorSet();
    mUboWrapper.emplace(mUboBuffer->getMemory());
    createGraphicsPipeline();
    recordCommandBuffers();
}

void KaleidoscopeRenderer::renderFrame() {
    UBO ubo{};
    ubo.projection = mProjectionMatrix;
    mUboWrapper->update(mVkContext->getVkDevice(), ubo);
}

void KaleidoscopeRenderer::recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t imageIndex) {
    // パイプラインバインド
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicsPipeline->getPipeline());

    // ディスクリプタセットバインド
    std::array<vk::DescriptorSet, 1> sets = {mDescriptorSet};
    cmdBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mGraphicsPipeline->getPipelineLayout(),
            0,
            sets,
            nullptr
    );

    // 頂点バッファとインスタンスバッファ
    vk::Buffer vertexBuffers[] = {mVertexBuffer->getBuffer()};
    vk::Buffer instanceBuffers[] = {mInstanceBuffer->getBuffer()};
    vk::DeviceSize offsets[] = {0};

    cmdBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
    cmdBuffer.bindVertexBuffers(1, 1, instanceBuffers, offsets);

    // インデックスバッファ
    cmdBuffer.bindIndexBuffer(mIndexBuffer->getBuffer(), 0, vk::IndexType::eUint16);

    // 描画
    auto indexCount = static_cast<uint32_t>(indices.size());
    cmdBuffer.drawIndexed(indexCount, 10, 0, 0, 0);

    LOGI("recordDrawCommands done for command buffer %d", imageIndex);
}

void KaleidoscopeRenderer::updateProjectionMatrix(uint32_t width, uint32_t height) {
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    float aspect = float(width) / float(height);
    projection[0][0] /= aspect;  // X 軸補正
    mProjectionMatrix = projection;
}

void KaleidoscopeRenderer::createBuffer() {
    vk::Device device = mVkContext->getVkDevice();
    vk::PhysicalDevice physicalDevice = mVkContext->getVkPhysicalDevice();

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    mVertexBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            bufferSize,
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    mVertexBuffer->copyFrom(vertices.data(), bufferSize);

    bufferSize = sizeof(indices[0]) * indices.size();

    /* Index Buffer */

    mIndexBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            bufferSize,
            vk::BufferUsageFlagBits::eIndexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    mIndexBuffer->copyFrom(indices.data(), bufferSize);

    /* インスタンシングの設定 */

    uint32_t instanceCount = 10;
    std::vector<InstanceData> instances(instanceCount);
    for (int i = 0; i < instanceCount; ++i) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i * 0.3f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        instances[i].model = model;
    }

    VkDeviceSize instanceBufferSize = sizeof(InstanceData) * instanceCount;

    mInstanceBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            instanceBufferSize,
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    mInstanceBuffer->copyFrom(instances.data(), instanceBufferSize);

    /* UBO Buffer */

    mUboBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            sizeof(UBO),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );
}

void KaleidoscopeRenderer::createGraphicsPipeline() {
    vk::Device device = mVkContext->getVkDevice();
    AAssetManager *assetManager = mVkContext->getAssetManager();

    std::vector<char> vertexShaderSrc = VulkanUtils::readFile(assetManager, "shaders/triangle.vert.spv");
    std::vector<char> fragmentShaderSrc = VulkanUtils::readFile(assetManager, "shaders/triangle.frag.spv");

    auto vertexShaderModule = VulkanUtils::createShaderModule(device, vertexShaderSrc);
    auto fragmentShaderModule = VulkanUtils::createShaderModule(device, fragmentShaderSrc);

    std::vector<vk::ShaderModule> shaders = {vertexShaderModule.get(), fragmentShaderModule.get()};
    mGraphicsPipeline = std::make_unique<GraphicsPipeline>(device,
                                                           createPipeLineConfig(),
                                                           mRenderPass->getVkRenderPass(),
                                                           shaders);
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

PipelineConfig KaleidoscopeRenderer::createPipeLineConfig() {
    // 頂点バッファ
    vk::VertexInputBindingDescription vertexBinding{};
    vertexBinding.binding = 0;
    vertexBinding.stride = sizeof(Vertex);
    vertexBinding.inputRate = vk::VertexInputRate::eVertex;

    // インスタンスバッファ
    vk::VertexInputBindingDescription instanceBinding{};
    instanceBinding.binding = 1;
    instanceBinding.stride = sizeof(InstanceData);
    instanceBinding.inputRate = vk::VertexInputRate::eInstance;

    std::vector<vk::VertexInputBindingDescription> bindings = {vertexBinding, instanceBinding};

    // 頂点属性
    vk::VertexInputAttributeDescription posAttr{};
    posAttr.location = 0;
    posAttr.binding = 0;
    posAttr.format = vk::Format::eR32G32Sfloat;
    posAttr.offset = offsetof(Vertex, pos);

    std::vector<vk::VertexInputAttributeDescription> attributes;

    attributes.push_back(posAttr);

    for (uint32_t i = 0; i < 4; ++i) {
        vk::VertexInputAttributeDescription instAttr{};
        instAttr.location = 1 + i;
        instAttr.binding = 1;
        instAttr.format = vk::Format::eR32G32B32A32Sfloat;
        instAttr.offset = offsetof(InstanceData, model) + sizeof(glm::vec4) * i;
        attributes.push_back(instAttr);
    }

    vk::PipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
    vertexInput.pVertexBindingDescriptions = bindings.data();
    vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
    vertexInput.pVertexAttributeDescriptions = attributes.data();

    PipelineConfig pipelineConfig{};
    pipelineConfig.vertexInputStateCreateInfo = vertexInput;

    return pipelineConfig;
}