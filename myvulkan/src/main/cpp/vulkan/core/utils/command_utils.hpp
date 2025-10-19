#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

class CommandUtils {

public:

    static vk::UniqueCommandBuffer beginSingleTimeCommands(VulkanContext &context) {
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool = context.getTransientCommandPool();
        allocInfo.commandBufferCount = 1;

        auto commandBuffer = std::move(context.getVkDevice().allocateCommandBuffersUnique(allocInfo).front());

        vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
        commandBuffer->begin(beginInfo);

        return commandBuffer;
    }

    static void endSingleTimeCommands(VulkanContext &context, vk::UniqueCommandBuffer commandBuffer) {
        commandBuffer->end();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        vk::CommandBuffer cbHandle = *commandBuffer;
        submitInfo.pCommandBuffers = &cbHandle;

        auto graphicsQueue = context.getDevice().getGraphicsQueue();

        graphicsQueue.submit(submitInfo, nullptr);
        graphicsQueue.waitIdle();
    }

};