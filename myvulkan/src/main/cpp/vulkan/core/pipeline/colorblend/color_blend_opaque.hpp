#pragma once

#include <vulkan/vulkan.hpp>

#include "color_blend.hpp"

/**
 * 不透明描画用
 */
class ColorBlendOpaque : public ColorBlend {
   public:
    ColorBlendOpaque() {
        attachment_ = vk::PipelineColorBlendAttachmentState{};
        attachment_.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                     vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        attachment_.blendEnable = VK_FALSE;

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
