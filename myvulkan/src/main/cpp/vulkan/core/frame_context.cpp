#include "frame_context.hpp"

FrameContext::FrameContext(VulkanContext &vkContext) : mVkContext(vkContext) {
    auto device = vkContext.getDevice();

    vk::SemaphoreCreateInfo semaphoreInfo{};
    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled; // フェンスの初期状態をシグナルにする

    mInFlightFences = device.createFenceUnique(fenceInfo);
    mImageAvailable = device.createSemaphoreUnique(semaphoreInfo);
    mRenderFinished = device.createSemaphoreUnique(semaphoreInfo);
    mCommandBuffer = createCommandBuffer();
}

vk::UniqueCommandBuffer FrameContext::createCommandBuffer() {
    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = mVkContext.getGraphicsCommandPool();
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = 1;

    auto device = mVkContext.getDevice();
    try {
        std::vector<vk::UniqueCommandBuffer> cmdBuffers = device.allocateCommandBuffersUnique(allocInfo);
        return std::move(cmdBuffers[0]);
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Failed to allocate command buffers: ") + e.what());
    }
}
