#pragma once

#include <vulkan/vulkan.hpp>

#include "color_blend.hpp"

/**
 * 透明描画用（アルファブレンド有効）
 */
class ColorBlendAlpha : public ColorBlend {
   public:
    ColorBlendAlpha() {
        attachment_ = vk::PipelineColorBlendAttachmentState{};
        attachment_.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                     vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        attachment_.blendEnable = VK_TRUE;
        attachment_.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        attachment_.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        attachment_.colorBlendOp = vk::BlendOp::eAdd;
        attachment_.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        attachment_.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        attachment_.alphaBlendOp = vk::BlendOp::eAdd;

        blendState_ = vk::PipelineColorBlendStateCreateInfo{};
        blendState_.logicOpEnable = VK_FALSE;
        blendState_.attachmentCount = 1;
        blendState_.pAttachments = &attachment_;
    }

    vk::PipelineColorBlendStateCreateInfo const& getBlendState() const override { return blendState_; }

    vk::PipelineColorBlendAttachmentState const& getAttachmentState() const override { return attachment_; }

   private:
    vk::PipelineColorBlendAttachmentState attachment_;
    vk::PipelineColorBlendStateCreateInfo blendState_;
};
