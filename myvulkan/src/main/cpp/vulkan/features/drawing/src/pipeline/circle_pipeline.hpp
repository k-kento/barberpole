#pragma once

#include "../input_vertex.hpp"
#include "base_pipeline.hpp"
#include "colorblend/color_blend_opaque.hpp"
#include "pipeline_builder.hpp"
#include "renderpass/render_pass.hpp"
#include "shader_helper.hpp"
#include "vulkan_context.h"

/**
 * CirclePipeline - 円ブラシ用パイプライン
 *
 * 特徴:
 * - Triangle List トポロジー（個別の三角形）
 * - Alpha Blending 有効化
 */
class CirclePipeline : public BasePipeline {
   public:
    CirclePipeline(VulkanContext& context, RenderPass& renderPass) : BasePipeline() {
        auto device = context.getDevice();
        mDescriptorSetLayout = createDescriptorSetLayout(device);
        mPipelineLayout = createPipelineLayout(device);
        mPipeline = createPipeline(context, renderPass);
    };

   private:
    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::Device device) {
        // 今回は UBO 不要だが将来用に残す
        vk::DescriptorSetLayoutBinding ubo{};
        ubo.binding = 0;
        ubo.descriptorType = vk::DescriptorType::eUniformBuffer;
        ubo.descriptorCount = 1;
        ubo.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;

        vk::DescriptorSetLayoutCreateInfo info{};
        info.bindingCount = 1;
        info.pBindings = &ubo;

        return device.createDescriptorSetLayoutUnique(info);
    }

    vk::UniquePipelineLayout createPipelineLayout(vk::Device device) {
        vk::DescriptorSetLayout layouts[] = {mDescriptorSetLayout.get()};

        vk::PipelineLayoutCreateInfo info{};
        info.setLayoutCount = 1;
        info.pSetLayouts = layouts;

        return device.createPipelineLayoutUnique(info);
    }

    vk::UniquePipeline createPipeline(VulkanContext& context, RenderPass& renderPass) {
        vk::Device device = context.getDevice();
        const std::string dir = "shaders/drawing/";

        auto vs = ShaderHelper::createShaderModule(context, dir + "circle.vert.spv");

        auto fs = ShaderHelper::createShaderModule(context, dir + "circle.frag.spv");

        auto stages = ShaderHelper::makeGraphicsStages(*vs, *fs);

        auto vertexInput = createVertexConfig();

        ColorBlendOpaque blendOpaque;

        auto builder =
            PipelineBuilder(stages, vertexInput, renderPass.getVkRenderPass(), mPipelineLayout.get(), blendOpaque);

        /* Raster */
        builder.rasterizer = vk::PipelineRasterizationStateCreateInfo{}
                                 .setPolygonMode(vk::PolygonMode::eFill)
                                 .setCullMode(vk::CullModeFlagBits::eNone)
                                 .setFrontFace(vk::FrontFace::eCounterClockwise)
                                 .setLineWidth(1.0f);

        /* ★ POINTS */
        builder.inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{}
                                    .setTopology(vk::PrimitiveTopology::ePointList)
                                    .setPrimitiveRestartEnable(VK_FALSE);

        return builder.build(device, nullptr);
    }

    vk::PipelineVertexInputStateCreateInfo createVertexConfig() {
        mBindings.clear();
        mAttributes.clear();

        vk::VertexInputBindingDescription bind{};
        bind.binding = 0;
        bind.stride = sizeof(InputVertex);
        bind.inputRate = vk::VertexInputRate::eVertex;
        mBindings.push_back(bind);

        /* pos */
        vk::VertexInputAttributeDescription pos{};
        pos.location = 0;
        pos.binding = 0;
        pos.format = vk::Format::eR32G32Sfloat;
        pos.offset = offsetof(InputVertex, position);
        mAttributes.push_back(pos);

        /* color */
        vk::VertexInputAttributeDescription col{};
        col.location = 2;
        col.binding = 0;
        col.format = vk::Format::eR32G32B32A32Sfloat;
        col.offset = offsetof(InputVertex, color);
        mAttributes.push_back(col);

        vk::PipelineVertexInputStateCreateInfo info{};
        info.vertexBindingDescriptionCount = static_cast<uint32_t>(mBindings.size());
        info.pVertexBindingDescriptions = mBindings.data();

        info.vertexAttributeDescriptionCount = static_cast<uint32_t>(mAttributes.size());
        info.pVertexAttributeDescriptions = mAttributes.data();

        return info;
    }
};
