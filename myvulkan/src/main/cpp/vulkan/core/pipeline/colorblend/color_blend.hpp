#pragma once

#include <vulkan/vulkan.hpp>

/**
 * Base class for Color Blend settings
 */
class ColorBlend {
   public:
    virtual ~ColorBlend() = default;

    // BlendStateCreateInfo を返す純粋仮想関数
    virtual vk::PipelineColorBlendStateCreateInfo const& getBlendState() const = 0;

    // AttachmentState を返す
    virtual vk::PipelineColorBlendAttachmentState const& getAttachmentState() const = 0;
};
