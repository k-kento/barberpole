//
// Created by 黒須権人 on 2025/09/27.
//

#pragma once

#include <memory>

#include "vulkan/vulkan.hpp"

class RenderPass {
   public:
    RenderPass(vk::Device device, vk::Format colorFormat);

    ~RenderPass();

    RenderPass(const RenderPass&) = delete;

    RenderPass& operator=(const RenderPass&) = delete;

    RenderPass(RenderPass&& other) noexcept;

    RenderPass& operator=(RenderPass&& other) noexcept;

    [[nodiscard]] vk::RenderPass getVkRenderPass() const { return mRenderPass.get(); }

   private:
    vk::UniqueRenderPass mRenderPass;
};