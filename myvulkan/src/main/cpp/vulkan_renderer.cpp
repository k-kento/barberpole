#include "vulkan_renderer.h"
#include "log.h"
#include <chrono>
#include <iostream>
#include <vulkan/vulkan_android.h>

VulkanRenderer::VulkanRenderer() {}

VulkanRenderer::~VulkanRenderer() { destroy(); }

bool VulkanRenderer::init(VulkanContext *vkContext, ANativeWindow *window) {
    mVkContext = vkContext;
    mSwapChain = new SwapChain(); // TODO
    mSurface = new Surface(); // TODO

    if (!mSurface->create(mVkContext, window)) return false;
    if (!mSwapChain->create(mVkContext, mSurface->getSurface())) return false;
    if (!createRenderPass()) return false;
    if (!createFramebuffers()) return false;
    if (!createCommandPoolAndBuffers()) return false;
    if (!recordCommandBuffers()) return false;
    if (!createSemaphores()) return false;

    LOGI("Vulkan initialized");
    return true;
}

void VulkanRenderer::start() {
    if (gRun.load()) return;
    gRun.store(true);
    gRenderThread = std::thread(&VulkanRenderer::renderLoop, this);
}

void VulkanRenderer::stop() {
    if (!gRun.load()) return;
    gRun.store(false);
    if (gRenderThread.joinable()) gRenderThread.join();
}

void VulkanRenderer::destroy() {
    VkDevice device = mVkContext->getDevice();
    stop();
    if (device != VK_NULL_HANDLE) vkDeviceWaitIdle(device);
    if (mSwapChain) mSwapChain->destroy(device);
    if (gImageAvailable) vkDestroySemaphore(device, gImageAvailable, nullptr);
    if (gRenderFinished) vkDestroySemaphore(device, gRenderFinished, nullptr);
    for (auto fb: gFramebuffers) if (fb) vkDestroyFramebuffer(device, fb, nullptr);
    for (auto iv: mSwapChain->getImageViews()) if (iv) vkDestroyImageView(device, iv, nullptr);
    if (gRenderPass) vkDestroyRenderPass(device, gRenderPass, nullptr);
    if (gCommandPool) vkDestroyCommandPool(device, gCommandPool, nullptr);
    if (mSurface) mSurface->destroy(mVkContext);

    LOGI("Vulkan destroyed");
}

void VulkanRenderer::renderLoop() {
    while (gRun.load()) {
        uint32_t imageIndex = 0;

        vkAcquireNextImageKHR(mVkContext->getDevice(), mSwapChain->getSwapChain(), UINT64_MAX, gImageAvailable,
                              VK_NULL_HANDLE,
                              &imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {gImageAvailable};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &gCmdBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = {gRenderFinished};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkQueueSubmit(mVkContext->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR scs[] = {mSwapChain->getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = scs;
        presentInfo.pImageIndices = &imageIndex;
        vkQueuePresentKHR(mVkContext->getGraphicsQueue(), &presentInfo);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

bool VulkanRenderer::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = mSwapChain->getFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorRef;

    VkRenderPassCreateInfo rpInfo{};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = 1;
    rpInfo.pAttachments = &colorAttachment;
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(mVkContext->getDevice(), &rpInfo, nullptr, &gRenderPass) != VK_SUCCESS) {
        LOGE("Failed to create render pass");
        return false;
    }
    return true;
}

bool VulkanRenderer::createFramebuffers() {
    std::vector<VkImageView> imageViews = mSwapChain->getImageViews();
    gFramebuffers.resize(imageViews.size());
    for (size_t i = 0; i < imageViews.size(); ++i) {
        VkImageView attachments[] = {imageViews[i]};

        VkFramebufferCreateInfo fbInfo{};
        fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbInfo.renderPass = gRenderPass;
        fbInfo.attachmentCount = 1;
        fbInfo.pAttachments = attachments;
        fbInfo.width = mSwapChain->getExtent().width;
        fbInfo.height = mSwapChain->getExtent().height;
        fbInfo.layers = 1;

        if (vkCreateFramebuffer(mVkContext->getDevice(), &fbInfo, nullptr, &gFramebuffers[i]) != VK_SUCCESS) {
            LOGE("Failed to create framebuffer %zu", i);
            return false;
        }
    }
    return true;
}

bool VulkanRenderer::createCommandPoolAndBuffers() {
    VkDevice device = mVkContext->getDevice();
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = mVkContext->getQueueFamilyIndex();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &gCommandPool) != VK_SUCCESS) {
        LOGE("Failed to create command pool");
        return false;
    }

    gCmdBuffers.resize(gFramebuffers.size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = gCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(gCmdBuffers.size());

    if (vkAllocateCommandBuffers(device, &allocInfo, gCmdBuffers.data()) != VK_SUCCESS) {
        LOGE("Failed to allocate command buffers");
        return false;
    }

    return true;
}

bool VulkanRenderer::recordCommandBuffers() {
    for (size_t i = 0; i < gCmdBuffers.size(); ++i) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(gCmdBuffers[i], &beginInfo);

        VkClearValue clearColor = {0.0f, 1.0f, 0.0f, 1.0f};
        VkRenderPassBeginInfo rpBegin{};
        rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.renderPass = gRenderPass;
        rpBegin.framebuffer = gFramebuffers[i];
        rpBegin.renderArea.offset = {0, 0};
        rpBegin.renderArea.extent = mSwapChain->getExtent();
        rpBegin.clearValueCount = 1;
        rpBegin.pClearValues = &clearColor;

        vkCmdBeginRenderPass(gCmdBuffers[i], &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(gCmdBuffers[i]);
        vkEndCommandBuffer(gCmdBuffers[i]);
    }
    return true;
}

bool VulkanRenderer::createSemaphores() {
    VkDevice device = mVkContext->getDevice();
    VkSemaphoreCreateInfo semInfo{};
    semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &semInfo, nullptr, &gImageAvailable) != VK_SUCCESS) return false;
    if (vkCreateSemaphore(device, &semInfo, nullptr, &gRenderFinished) != VK_SUCCESS) return false;
    return true;
}
