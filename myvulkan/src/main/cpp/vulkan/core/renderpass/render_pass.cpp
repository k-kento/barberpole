#include "render_pass.hpp"

#include <stdexcept>

#include "log.h"
#include "vulkan/vulkan.hpp"

RenderPass::RenderPass(vk::Device device, vk::Format colorFormat) {
    // カラーバッファアタッチメント
    vk::AttachmentDescription colorAttachment({},
                                              colorFormat,
                                              vk::SampleCountFlagBits::e1,
                                              vk::AttachmentLoadOp::eClear,
                                              vk::AttachmentStoreOp::eStore,
                                              vk::AttachmentLoadOp::eDontCare,
                                              vk::AttachmentStoreOp::eDontCare,
                                              vk::ImageLayout::eUndefined,
                                              vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference colorRef(0, vk::ImageLayout::eColorAttachmentOptimal);

    // Subpass
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorRef);

    vk::RenderPassCreateInfo rpInfo({}, 1, &colorAttachment, 1, &subpass);

    try {
        mRenderPass = device.createRenderPassUnique(rpInfo);
    } catch (const vk::SystemError& err) {
        LOGE("Failed to init render pass: %s", err.what());
    }

    LOGI("RenderPass created.");
}

RenderPass::~RenderPass() {
    LOGI("RenderPass destroyed");
}

RenderPass::RenderPass(RenderPass&& other) noexcept : mRenderPass(std::move(other.mRenderPass)) {}

RenderPass& RenderPass::operator=(RenderPass&& other) noexcept {
    if (this != &other) {
        mRenderPass = std::move(other.mRenderPass);
    }
    return *this;
}
