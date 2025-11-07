
#include "pipeline_config.hpp"
#include "pipeline_builder.hpp"
#include "shader_helper.hpp"
#include "vertex.hpp"
#include "instance_buffer.hpp"

vk::UniquePipeline
PipelineConfig::createPipeline(VulkanContext &context,
                               vk::PipelineLayout &pipelineLayout,
                               RenderPass &renderPass) {

    vk::Device device = context.getDevice();
    AAssetManager *assetManager = context.getAssetManager();

    const std::string directory = "shaders/drawing/";

    auto vertexShaderModule = ShaderHelper::createShaderModule(context, directory + "drawing.vert.spv");
    auto geometryShaderModule = ShaderHelper::createShaderModule(context, directory + "drawing.geom.spv");
    auto fragmentShaderModule = ShaderHelper::createShaderModule(context, directory + "drawing.frag.spv");

    auto shaderStages = createShaderStages(vertexShaderModule.get(), geometryShaderModule.get(), fragmentShaderModule.get());
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

    mBindings.push_back(vertexBinding);
    mBindings.push_back(instanceBinding);

    // 位置属性
    vk::VertexInputAttributeDescription posAttr{};
    posAttr.location = 0;
    posAttr.binding = 0;
    posAttr.format = vk::Format::eR32G32Sfloat;
    posAttr.offset = offsetof(drawing::Vertex, position);
    mAttributes.push_back(posAttr);

    // instance
    for (uint32_t i = 0; i < 4; ++i) {
        vk::VertexInputAttributeDescription instAttr{};
        instAttr.location = 1 + i;
        instAttr.binding = 1;
        instAttr.format = vk::Format::eR32G32B32A32Sfloat;
        instAttr.offset = offsetof(InstanceData, model) + sizeof(glm::vec4) * i;
        mAttributes.push_back(instAttr);
    }

    vk::PipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(mBindings.size());
    vertexInput.pVertexBindingDescriptions = mBindings.data();
    vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(mAttributes.size());
    vertexInput.pVertexAttributeDescriptions = mAttributes.data();

    return vertexInput;
}

std::vector<vk::PipelineShaderStageCreateInfo>
PipelineConfig::createShaderStages(vk::ShaderModule &vertexModule, vk::ShaderModule &geometryModule, vk::ShaderModule &fragmentModule) {
    vk::PipelineShaderStageCreateInfo vertStage{};
    vertStage.stage = vk::ShaderStageFlagBits::eVertex;
    vertStage.module = vertexModule;
    vertStage.pName = "main";

    vk::PipelineShaderStageCreateInfo geometryStage{};
    geometryStage.stage = vk::ShaderStageFlagBits::eGeometry;
    geometryStage.module = geometryModule;
    geometryStage.pName = "main";

    vk::PipelineShaderStageCreateInfo fragStage{};
    fragStage.stage = vk::ShaderStageFlagBits::eFragment;
    fragStage.module = fragmentModule;
    fragStage.pName = "main";

    return {vertStage, geometryStage, fragStage};
}
