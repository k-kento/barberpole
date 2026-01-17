#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

class CommandBufferHelper {
   public:
    static vk::UniqueCommandBuffer createCommandBuffer(VulkanContext& context) {
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.commandPool = context.getGraphicsCommandPool();
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;

        auto device = context.getDevice();
        try {
            std::vector<vk::UniqueCommandBuffer> cmdBuffers = device.allocateCommandBuffersUnique(allocInfo);
            return std::move(cmdBuffers[0]);
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Failed to allocate command buffers: ") + e.what());
        }
    }

    static vk::UniqueCommandBuffer beginSingleTimeCommands(VulkanContext& context) {
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool = context.getTransientCommandPool();
        allocInfo.commandBufferCount = 1;

        auto commandBuffer = std::move(context.getDevice().allocateCommandBuffersUnique(allocInfo).front());

        vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
        commandBuffer->begin(beginInfo);

        return commandBuffer;
    }

    static void endSingleTimeCommands(VulkanContext& context, vk::UniqueCommandBuffer commandBuffer) {
        commandBuffer->end();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        vk::CommandBuffer cbHandle = *commandBuffer;
        submitInfo.pCommandBuffers = &cbHandle;

        auto graphicsQueue = context.getGraphicsQueue();

        graphicsQueue.submit(submitInfo, nullptr);
        graphicsQueue.waitIdle();
    }
};
