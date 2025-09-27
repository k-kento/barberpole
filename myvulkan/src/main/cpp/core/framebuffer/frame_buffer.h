#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "swap_chain.h"
#include "render_pass.h"

class FrameBuffer {
public:
    FrameBuffer(VkDevice device);

    ~FrameBuffer();

    FrameBuffer(const FrameBuffer &) = delete;

    FrameBuffer &operator=(const FrameBuffer &) = delete;

    FrameBuffer(FrameBuffer &&other) noexcept;

    FrameBuffer &operator=(FrameBuffer &&other) noexcept;

    bool init(SwapChain *swapChain, RenderPass *renderPass);

    std::vector<VkFramebuffer> &getFrameBuffers() { return mFrameBuffers; }

private:
    VkDevice mDevice = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> mFrameBuffers;

    void destroy();
};
