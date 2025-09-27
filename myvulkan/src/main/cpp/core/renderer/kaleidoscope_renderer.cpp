#include <vulkan/vulkan.h>
#include "kaleidoscope_renderer.h"
#include "vulkan_utils.h"
#include "log.h"
#include "shader_module.h"
#include "graphics_pipeline.h"
#include "vulkan_buffer.h"

bool KaleidoscopeRenderer::init(VulkanContext *vkContext, ANativeWindow *window) {
    if (!VulkanRenderer::init(vkContext, window)) return false;

    auto device = vkContext->getDevice()->getDevice();
    auto physicalDevice = vkContext->getPhysicalDevice()->getPhysicalDevice();

    const std::vector<Vertex> vertices = {
            {{0.0f,  -0.5f}},
            {{0.5f,  0.5f}},
            {{-0.5f, 0.5f}},
    };

    VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

    mVertexBuffer = std::make_unique<VulkanBuffer>(
            device,
            physicalDevice,
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    mVertexBuffer->copyFrom(vertices.data(), bufferSize);

    AAssetManager *assetManager = mVkContext->getAssetManager();

    std::vector<char> vertexShaderSrc = VulkanUtils::readFile(assetManager, "shaders/triangle.vert.spv");
    std::vector<char> fragmentShaderSrc = VulkanUtils::readFile(assetManager, "shaders/triangle.frag.spv");

    ShaderModule vertShader(device, vertexShaderSrc);
    ShaderModule fragShader(device, fragmentShaderSrc);

    mGraphicsPipeline = std::make_unique<GraphicsPipeline>(device);
    mGraphicsPipeline->init(mRenderPass->getVkRenderPass(), {vertShader.get(), fragShader.get()});

    if (!VulkanRenderer::postInit()) return false;

    return true;
}

void KaleidoscopeRenderer::recordDrawCommands(VkCommandBuffer cmdBuffer, uint32_t imageIndex) {
    // パイプラインバインド
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline->getPipeline());

    // 頂点バッファバインド
    VkBuffer vertexBuffers[] = {mVertexBuffer->getBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);

    // 三角形描画
    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
}
