#pragma once

#include "../input_vertex.hpp"
#include "base_pipeline.hpp"
#include "pipeline_builder.hpp"
#include "renderpass/render_pass.hpp"
#include "shader_helper.hpp"
#include "vulkan_context.h"

/**
 * GlowPipeline - グローエフェクト用パイプライン
 *
 * 特徴:
 * - UV 頂点属性を追加（グロー計算用）
 * - UBO を Vertex + Fragment シェーダーで共有
 * - Alpha Blending 有効化
 */
class GlowPipeline : public BasePipeline {
   public:
    GlowPipeline(VulkanContext& context, RenderPass& renderPass) : BasePipeline() {
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
        // Vertex + Fragment 両方からアクセス可能
        uboBinding.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;

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

    vk::UniquePipeline createPipeline(VulkanContext& context, RenderPass& renderPass) {
        vk::Device device = context.getDevice();

        const std::string directory = "shaders/drawing/";

        auto vertexShaderModule = ShaderHelper::createShaderModule(context, directory + "glow.vert.spv");
        auto fragmentShaderModule = ShaderHelper::createShaderModule(context, directory + "glow.frag.spv");
        auto shaderStages = ShaderHelper::makeGraphicsStages(*vertexShaderModule, *fragmentShaderModule);

        ColorBlendOpaque blendOpaque;

        auto vertexInputInfo = createVertexConfig();
        auto builder = PipelineBuilder(
            shaderStages, vertexInputInfo, renderPass.getVkRenderPass(), mPipelineLayout.get(), blendOpaque);

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
        vk::VertexInputBindingDescription vertexBinding{};
        vertexBinding.binding = 0;
        vertexBinding.stride = sizeof(InputVertex);
        vertexBinding.inputRate = vk::VertexInputRate::eVertex;

        mBindings.push_back(vertexBinding);

        // Position (location 0)
        vk::VertexInputAttributeDescription posAttr{};
        posAttr.location = 0;
        posAttr.binding = 0;
        posAttr.format = vk::Format::eR32G32Sfloat;
        posAttr.offset = offsetof(InputVertex, position);
        mAttributes.push_back(posAttr);

        // Color (location 1)
        vk::VertexInputAttributeDescription colorAttr{};
        colorAttr.binding = 0;
        colorAttr.location = 1;
        colorAttr.format = vk::Format::eR32G32B32A32Sfloat;
        colorAttr.offset = offsetof(InputVertex, color);
        mAttributes.push_back(colorAttr);

        // UV (location 2) - グロー計算用
        vk::VertexInputAttributeDescription uvAttr{};
        uvAttr.binding = 0;
        uvAttr.location = 2;
        uvAttr.format = vk::Format::eR32G32Sfloat;
        uvAttr.offset = offsetof(InputVertex, uv);
        mAttributes.push_back(uvAttr);

        vk::PipelineVertexInputStateCreateInfo vertexInput{};
        vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(mBindings.size());
        vertexInput.pVertexBindingDescriptions = mBindings.data();
        vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(mAttributes.size());
        vertexInput.pVertexAttributeDescriptions = mAttributes.data();

        return vertexInput;
    }
};
