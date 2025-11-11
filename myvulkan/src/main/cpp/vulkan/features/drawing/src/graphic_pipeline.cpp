
#include "graphic_pipeline.hpp"
#include "pipeline_builder.hpp"
#include "shader_helper.hpp"
#include "input_vertex.hpp"
#include "input_buffer.hpp"

GraphicPipeline::GraphicPipeline(VulkanContext &context, RenderPass &renderPass, GraphicDescriptor &graphicDescriptor) {
    mPipelineLayout = createPipelineLayout(context, graphicDescriptor);
    mPipeline = createPipeline(context, renderPass);
}

vk::UniquePipelineLayout GraphicPipeline::createPipelineLayout(VulkanContext &context, GraphicDescriptor &graphicDescriptor) {
    vk::PushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset = 0;

    vk::DescriptorSetLayout layouts[] = {graphicDescriptor.getLayout()};
    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = layouts;
    mPipelineLayout = context.getDevice().createPipelineLayoutUnique(layoutInfo);
    mPipeline = createPipeline(context, renderPass);
    return context.getDevice().createPipelineLayoutUnique(layoutInfo);
}

vk::UniquePipeline
GraphicPipeline::createPipeline(VulkanContext &context, RenderPass &renderPass) {

    vk::Device device = context.getDevice();
    AAssetManager *assetManager = context.getAssetManager();

    const std::string directory = "shaders/drawing/";

    auto vertexShaderModule = ShaderHelper::createShaderModule(context, directory + "drawing.vert.spv");
    auto fragmentShaderModule = ShaderHelper::createShaderModule(context, directory + "rainbow.frag.spv");

    auto shaderStages = createShaderStages(vertexShaderModule.get(), fragmentShaderModule.get());
    auto vertexInputInfo = createVertexConfig();
    auto builder = PipelineBuilder(shaderStages, vertexInputInfo, renderPass.getVkRenderPass(), mPipelineLayout.get());

    builder.rasterizer = vk::PipelineRasterizationStateCreateInfo{}
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setCullMode(vk::CullModeFlagBits::eNone)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE)
            .setLineWidth(1.0f);

    builder.inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{}
            .setTopology(vk::PrimitiveTopology::eTriangleStrip)
            .setPrimitiveRestartEnable(VK_FALSE);

    return builder.build(context.getDevice(), nullptr);
}

vk::PipelineVertexInputStateCreateInfo GraphicPipeline::createVertexConfig() {
    // 頂点バッファ
    vk::VertexInputBindingDescription vertexBinding{};
    vertexBinding.binding = 0;
    vertexBinding.stride = sizeof(InputVertex);
    vertexBinding.inputRate = vk::VertexInputRate::eVertex;

    mBindings.push_back(vertexBinding);

    // Position (location = 0)
    vk::VertexInputAttributeDescription posAttr{};
    posAttr.location = 0;
    posAttr.binding = 0;
    posAttr.format = vk::Format::eR32G32Sfloat;
    posAttr.offset = offsetof(InputVertex, position);
    mAttributes.push_back(posAttr);

    // Color (location = 1)
    vk::VertexInputAttributeDescription colorAttr{};
    colorAttr.binding = 0;
    colorAttr.location = 1;
    colorAttr.format = vk::Format::eR32G32B32A32Sfloat;
    colorAttr.offset = offsetof(InputVertex, color);
    mAttributes.push_back(colorAttr);

    vk::PipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(mBindings.size());
    vertexInput.pVertexBindingDescriptions = mBindings.data();
    vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(mAttributes.size());
    vertexInput.pVertexAttributeDescriptions = mAttributes.data();

    return vertexInput;
}

std::vector<vk::PipelineShaderStageCreateInfo>
GraphicPipeline::createShaderStages(vk::ShaderModule &vertexModule, vk::ShaderModule &fragmentModule) {
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
