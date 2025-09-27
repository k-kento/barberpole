#include "frame_buffer.h"
#include "log.h"
#include <stdexcept>
#include <swap_chain.h>
#include <render_pass.h>

FrameBuffer::FrameBuffer(VkDevice device) : mDevice(device) {
}

FrameBuffer::~FrameBuffer() {
    destroy();
}

FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
        : mDevice(other.mDevice), mFrameBuffers(std::move(other.mFrameBuffers)) {
    other.mFrameBuffers.clear();
}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other) noexcept {
    if (this != &other) {
        destroy();
        mDevice = other.mDevice;
        mFrameBuffers = std::move(other.mFrameBuffers);
        other.mFrameBuffers.clear();
    }
    return *this;
}

bool FrameBuffer::init(SwapChain *swapChain, RenderPass *renderPass) {
    if (!swapChain || !renderPass) {
        LOGE("SwapChain or RenderPass is null");
        return false;
    }

    std::vector<VkImageView> imageViews = swapChain->getImageViews();
    mFrameBuffers.resize(imageViews.size());
    for (size_t i = 0; i < imageViews.size(); ++i) {
        VkImageView attachments[] = {imageViews[i]};

        VkFramebufferCreateInfo fbInfo{};
        fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbInfo.renderPass = renderPass->getVkRenderPass();
        fbInfo.attachmentCount = 1;
        fbInfo.pAttachments = attachments;
        fbInfo.width = swapChain->getExtent().width;
        fbInfo.height = swapChain->getExtent().height;
        fbInfo.layers = 1;

        auto result = vkCreateFramebuffer(mDevice, &fbInfo, nullptr, &mFrameBuffers[i]);
        if (result != VK_SUCCESS) {
            LOGE("Failed to init framebuffer %zu", i);
            destroy();
            return false;
        }
    }
    return true;
}

void FrameBuffer::destroy() {
    for (auto &fb: mFrameBuffers) {
        if (fb != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(mDevice, fb, nullptr);
            fb = VK_NULL_HANDLE;
        }
    }
    mFrameBuffers.clear();
}
