#pragma once

#include "command_executor.hpp"
#include "vulkan_utils.h"

CommandExecutor::CommandExecutor(VulkanContext &context,
                                 RenderPass &renderPass,
                                 SwapChain &swapChain,
                                 RenderStrategy &strategy)
        : mContext(context), mRendererStrategy(strategy) {

    uint32_t queueFamilyIndex = context.getPhysicalDevice().getQueueFamilyIndex();
    auto device = context.getVkDevice();

    mFrameBuffers = VulkanUtils::createFrameBuffers(device, &swapChain, renderPass.getVkRenderPass());
    auto commandPool = mContext.getGraphicsCommandPool();
    mCmdBuffers = createCommandBuffers(device, commandPool, mFrameBuffers.size());

    recordCommandBuffers(renderPass, swapChain);
}

void CommandExecutor::renderFrame(
        SwapChain &swapChain,
        vk::Semaphore imageAvailable,
        vk::Semaphore renderFinished) {
    auto device = mContext.getVkDevice();
    auto graphicsQueue = mContext.getDevice().getGraphicsQueue();
    auto swapChainKHR = swapChain.getSwapChain();

    // 次に描画する SwapChain イメージを取得
    auto acquireResult = device.acquireNextImageKHR(
            swapChainKHR,
            UINT64_MAX,
            imageAvailable,
            nullptr
    );

    uint32_t imageIndex = acquireResult.value;

    mRendererStrategy.renderFrame();

    // コマンドバッファをキューに流す
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};


    vk::SubmitInfo submitInfo{};
    submitInfo.setWaitSemaphores(imageAvailable)
            .setWaitDstStageMask(waitStages)
            .setCommandBuffers(mCmdBuffers[imageIndex].get())
            .setSignalSemaphores(renderFinished);

    graphicsQueue.submit(submitInfo, nullptr);

    // 表示
    vk::SwapchainKHR swapChains[] = {swapChainKHR};
    vk::PresentInfoKHR presentInfo{};
    presentInfo.setWaitSemaphores(renderFinished)
            .setSwapchains(swapChains)
            .setImageIndices(imageIndex);

    graphicsQueue.presentKHR(presentInfo);
}

void CommandExecutor::recordCommandBuffers(RenderPass &renderPass, SwapChain &swapChain) {
    auto extent = swapChain.getExtent();
    for (size_t i = 0; i < mCmdBuffers.size(); ++i) {
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        mCmdBuffers[i]->begin(beginInfo);

        vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        vk::RenderPassBeginInfo rpBegin{};
        rpBegin.renderPass = renderPass.getVkRenderPass();
        rpBegin.framebuffer = mFrameBuffers[i].get();
        rpBegin.renderArea.extent = extent;
        rpBegin.clearValueCount = 1;
        rpBegin.pClearValues = &clearColor;

        mCmdBuffers[i]->beginRenderPass(rpBegin, vk::SubpassContents::eInline);

        vk::Viewport viewport{0, 0, (float) extent.width, (float) extent.height, 0.0f, 1.0f};
        mCmdBuffers[i]->setViewport(0, viewport);
        vk::Rect2D scissor{{0, 0}, extent};
        mCmdBuffers[i]->setScissor(0, scissor);

        mRendererStrategy.recordDrawCommands(mCmdBuffers[i].get());

        mCmdBuffers[i]->endRenderPass();
        mCmdBuffers[i]->end();
    }
}

std::vector<vk::UniqueCommandBuffer>
CommandExecutor::createCommandBuffers(vk::Device &device, vk::CommandPool &commandPool, size_t count) {
    std::vector<vk::UniqueCommandBuffer> cmdBuffers;

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    try {
        cmdBuffers = device.allocateCommandBuffersUnique(allocInfo);
    } catch (const vk::SystemError &e) {
        throw std::runtime_error(std::string("Failed to allocate command buffers: ") + e.what());
    }

    return cmdBuffers;
}
