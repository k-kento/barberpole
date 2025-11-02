
#include "pipeline_config.hpp"
#include "pipeline_builder.hpp"
#include "vulkan_utils.h"
#include "vertex.hpp"
#include "instance_buffer.hpp"

// TODO 要見直し
vk::UniquePipeline
PipelineConfig::createPipeline(VulkanContext &context,
                               vk::PipelineLayout &pipelineLayout,
                               RenderPass &renderPass) {

    vk::Device device = context.getDevice();
    AAssetManager *assetManager = context.getAssetManager();

    std::vector<char> vertexShaderSrc = VulkanUtils::readTextFile(assetManager, "shaders/drawing.vert.spv");
    std::vector<char> fragmentShaderSrc = VulkanUtils::readTextFile(assetManager, "shaders/drawing.frag.spv");

    auto vertexShaderModule = VulkanUtils::createShaderModule(device, vertexShaderSrc);
    auto fragmentShaderModule = VulkanUtils::createShaderModule(device, fragmentShaderSrc);

    auto shaderStages = createShaderStages(vertexShaderModule.get(), fragmentShaderModule.get());
    auto vertexInputInfo = createVertexConfig();
    auto builder = PipelineBuilder(shaderStages, vertexInputInfo, renderPass.getVkRenderPass(), pipelineLayout);

    builder.rasterizer = vk::PipelineRasterizationStateCreateInfo{}
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setCullMode(vk::CullModeFlagBits::eNone)
            .setDepthBiasEnable(VK_FALSE)
            .setLineWidth(1.0f);

    return builder.build(context.getDevice(), nullptr);
}

vk::PipelineVertexInputStateCreateInfo PipelineConfig::createVertexConfig() {
    // 頂点バッファ
    vk::VertexInputBindingDescription vertexBinding{};
    vertexBinding.binding = 0;
    vertexBinding.stride = sizeof(drawing::Vertex);
    vertexBinding.inputRate = vk::VertexInputRate::eVertex;

    // インスタンスバッファ
    vk::VertexInputBindingDescription instanceBinding{};
    instanceBinding.binding = 1;
    instanceBinding.stride = sizeof(InstanceData);
    instanceBinding.inputRate = vk::VertexInputRate::eInstance;

    std::vector<vk::VertexInputBindingDescription> bindings = {vertexBinding, instanceBinding};

    std::vector<vk::VertexInputAttributeDescription> attributes;

    // 位置属性
    vk::VertexInputAttributeDescription posAttr{};
    posAttr.location = 0;
    posAttr.binding = 0;
    posAttr.format = vk::Format::eR32G32Sfloat;
    posAttr.offset = offsetof(drawing::Vertex, position);
    attributes.push_back(posAttr);

    // instance
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

    return vertexInput;
}

std::vector<vk::PipelineShaderStageCreateInfo>
PipelineConfig::createShaderStages(vk::ShaderModule &vertexModule, vk::ShaderModule &fragmentModule) {
    vk::PipelineShaderStageCreateInfo vertStage{};
    vertStage.stage = vk::ShaderStageFlagBits::eVertex;
    vertStage.module = vertexModule;
    vertStage.pName = "main";

    vk::PipelineShaderStageCreateInfo fragStage{};
    fragStage.stage = vk::ShaderStageFlagBits::eFragment;
    fragStage.module = fragmentModule;
    fragStage.pName = "main";

    return {vertStage, fragStage};
}
