#include "pipeline_builder.hpp"

#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.hpp>

PipelineBuilder::PipelineBuilder(const std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages,
                                 const vk::PipelineVertexInputStateCreateInfo& vertexInputInfo,
                                 const vk::RenderPass renderPass,
                                 const vk::PipelineLayout pipelineLayout,
                                 ColorBlend& colorBlend)
    : shaderStages(shaderStages),
      vertexInputInfo(vertexInputInfo),
      mRenderPass(renderPass),
      layout(pipelineLayout),
      colorBlend(colorBlend) {
    inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{}
                        .setTopology(vk::PrimitiveTopology::eTriangleList)
                        .setPrimitiveRestartEnable(VK_FALSE);

    viewportState = vk::PipelineViewportStateCreateInfo{}.setViewportCount(1).setScissorCount(1);

    rasterizer = vk::PipelineRasterizationStateCreateInfo{}
                     .setDepthClampEnable(VK_FALSE)
                     .setRasterizerDiscardEnable(VK_FALSE)
                     .setPolygonMode(vk::PolygonMode::eFill)
                     .setCullMode(vk::CullModeFlagBits::eBack)
                     .setFrontFace(vk::FrontFace::eClockwise)
                     .setDepthBiasEnable(VK_FALSE)
                     .setLineWidth(1.0f);

    multisampling = vk::PipelineMultisampleStateCreateInfo{};

    depthStencil = vk::PipelineDepthStencilStateCreateInfo{}
                       .setDepthTestEnable(VK_TRUE)
                       .setDepthWriteEnable(VK_TRUE)
                       .setDepthCompareOp(vk::CompareOp::eLess)
                       .setStencilTestEnable(VK_FALSE);

    dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

    dynamicState = vk::PipelineDynamicStateCreateInfo{}.setDynamicStates(dynamicStates);
}

vk::UniquePipeline PipelineBuilder::build(vk::Device device, vk::PipelineCache cache) {
    if (shaderStages.empty()) throw std::runtime_error("PipelineBuilder: shaderStages not set");
    if (!layout) throw std::runtime_error("PipelineBuilder: layout not set");
    if (!mRenderPass) throw std::runtime_error("PipelineBuilder: renderPass not set");

    auto colorBlending = colorBlend.getBlendState();

    vk::GraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.setStages(shaderStages)
        .setPVertexInputState(&vertexInputInfo)
        .setPInputAssemblyState(&inputAssembly)
        .setPViewportState(&viewportState)
        .setPRasterizationState(&rasterizer)
        .setPMultisampleState(&multisampling)
        .setPDepthStencilState(&depthStencil)
        .setPColorBlendState(&colorBlending)
        .setPDynamicState(&dynamicState)
        .setLayout(layout)
        .setRenderPass(mRenderPass)
        .setSubpass(subpass);

    auto result = device.createGraphicsPipelineUnique(nullptr, pipelineInfo);
    if (result.result != vk::Result::eSuccess) throw std::runtime_error("Failed to create graphics pipeline");

    return std::move(result.value);
}
