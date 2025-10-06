#include "graphics_pipeline.h"
#include "vulkan_context.h"
#include <stdexcept>
#include <array>
#include "log.h"
#include "kaleidoscope_renderer.h"


GraphicsPipeline::GraphicsPipeline(vk::Device &device,
                                   const PipelineConfig &pipelineConfig,
                                   vk::RenderPass &renderPass,
                                   const std::vector<vk::ShaderModule> &shaderModules) {
    if (shaderModules.size() < 2) throw std::runtime_error("Need at least 2 shader modules.");

    vk::PipelineLayoutCreateInfo layoutInfo{};
    mPipelineLayout = device.createPipelineLayoutUnique(layoutInfo);

    if (!createGraphicsPipeline(device, renderPass, shaderModules, pipelineConfig))
        throw std::runtime_error("Failed to create graphics pipeline.");

    LOGI("Graphics pipeline created successfully.");
}


std::vector<vk::PipelineShaderStageCreateInfo>
GraphicsPipeline::createShaderStages(const std::vector<vk::ShaderModule> &shaderModules) {
    vk::PipelineShaderStageCreateInfo vertStage{};
    vertStage.stage = vk::ShaderStageFlagBits::eVertex;
    vertStage.module = shaderModules[0];
    vertStage.pName = "main";

    vk::PipelineShaderStageCreateInfo fragStage{};
    fragStage.stage = vk::ShaderStageFlagBits::eFragment;
    fragStage.module = shaderModules[1];
    fragStage.pName = "main";

    return {vertStage, fragStage};
}


bool GraphicsPipeline::createGraphicsPipeline(vk::Device &device, vk::RenderPass &renderPass,
                                              const std::vector<vk::ShaderModule> &shaderModules,
                                              const PipelineConfig &pipelineConfig) {
    LOGI("Creating Graphics Pipeline...");

    auto shaderStages = createShaderStages(shaderModules);
    LOGI("Shader stages created: %zu", shaderStages.size());

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = false;

    vk::PipelineViewportStateCreateInfo viewportState{};
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;


    // ラスタライザの設定
    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.polygonMode = vk::PolygonMode::eFill; // 塗りつぶしモード
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
    rasterizer.lineWidth = 1.0f;

    // マルチサンプルレンダリングの設定
    vk::PipelineMultisampleStateCreateInfo multisampling{};
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    /* Color */
    vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.colorWriteMask =
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA;

    colorBlendAttachment.blendEnable = false;

    vk::PipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;


    std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();


    vk::GraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &pipelineConfig.vertexInputStateCreateInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = mPipelineLayout.get();
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;


    LOGI("Calling createGraphicsPipelineUnique...");

    try {
        auto result = device.createGraphicsPipelineUnique(nullptr, pipelineInfo);
        mPipeline = std::move(result.value);
        LOGI("Graphics pipeline created successfully.");
    } catch (const vk::SystemError &e) {
        LOGE("Failed to create graphics pipeline: %s", e.what());
        return false;
    } catch (const std::exception &e) {
        LOGE("Other exception: %s", e.what());
        return false;
    }

    return true;
}
