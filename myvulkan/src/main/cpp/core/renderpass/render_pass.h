//
// Created by 黒須権人 on 2025/09/27.
//

#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class RenderPass {
public:
    RenderPass(VkDevice device);

    ~RenderPass();

    RenderPass(const RenderPass &) = delete;

    RenderPass &operator=(const RenderPass &) = delete;

    RenderPass(RenderPass &&other) noexcept;

    RenderPass &operator=(RenderPass &&other) noexcept;

    VkRenderPass getVkRenderPass() const { return mRenderPass; }

    bool init(VkFormat colorFormat);

private:
    VkDevice mDevice = VK_NULL_HANDLE;
    VkRenderPass mRenderPass = VK_NULL_HANDLE;

    void destroy();
};