#pragma once

#include "vulkan_context.h"
#include "render_pass.h"
#include "../graphic_descriptor.hpp"
#include "base_pipeline.hpp"
#include "../brush/normal/normal_brush.hpp"
#include "shader_helper.hpp"
#include "pipeline_builder.hpp"

class NormalPipeline : public BasePipeline {
public:

    NormalPipeline(VulkanContext &context, RenderPass &renderPass) : BasePipeline() {
        auto device = context.getDevice();
        mDescriptorSetLayout = createDescriptorSetLayout(device);
        mPipelineLayout = createPipelineLayout(device);
        mPipeline = createPipeline(context, renderPass);
    };

    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::Device device) {
        vk::DescriptorSetLayoutBinding uboBinding{};
        uboBinding.binding = 0;
        uboBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
        uboBinding.descriptorCount = 1;
        uboBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboBinding;

        return device.createDescriptorSetLayoutUnique(layoutInfo);
    }

    vk::UniquePipelineLayout createPipelineLayout(vk::Device device) {
        vk::DescriptorSetLayout layouts[] = {mDescriptorSetLayout.get()};
        vk::PipelineLayoutCreateInfo layoutInfo{};
        layoutInfo.setLayoutCount = 1;
        layoutInfo.pSetLayouts = layouts;
        return device.createPipelineLayoutUnique(layoutInfo);
    }

    vk::UniquePipeline
    createPipeline(VulkanContext &context, RenderPass &renderPass) {
        vk::Device device = context.getDevice();
        AAssetManager *assetManager = context.getAssetManager();

        const std::string directory = "shaders/drawing/";

        auto vertexShaderModule = ShaderHelper::createShaderModule(context, directory + "drawing.vert.spv");
        auto fragmentShaderModule = ShaderHelper::createShaderModule(context, directory + "rainbow.frag.spv");
        auto shaderStages = ShaderHelper::makeGraphicsStages(*vertexShaderModule, *fragmentShaderModule);

        auto vertexInputInfo = createVertexConfig();
        auto builder = PipelineBuilder(shaderStages,
                                       vertexInputInfo,
                                       renderPass.getVkRenderPass(),
                                       mPipelineLayout.get());

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

    vk::PipelineVertexInputStateCreateInfo createVertexConfig() {
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
};
