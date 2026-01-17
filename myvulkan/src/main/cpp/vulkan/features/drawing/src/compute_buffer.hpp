#pragma once

#include "input_vertex.hpp"
#include "log.h"
#include "renderer_constants.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

class ComputeBuffer {
   public:
    ComputeBuffer(VulkanContext& vkContext) {
        auto device = vkContext.getDevice();

        vk::BufferCreateInfo bufferInfo{};
        bufferInfo.size = sizeof(InputVertex) * MAX_COMPUTE_OUTPUT_VERTICES;
        bufferInfo.usage = vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        mSharedBuffer = device.createBufferUnique(bufferInfo);
        vk::MemoryRequirements memReq = device.getBufferMemoryRequirements(mSharedBuffer.get());

        auto physicalDevice = vkContext.getPhysicalDevice();

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memReq.size;
        allocInfo.memoryTypeIndex = PhysicalDeviceHelper::findMemoryType(
            physicalDevice, memReq.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        mSharedMemory = device.allocateMemoryUnique(allocInfo);
        device.bindBufferMemory(mSharedBuffer.get(), mSharedMemory.get(), 0);
    }

    [[nodiscard]] vk::Buffer getBuffer() const { return mSharedBuffer.get(); }

   private:
    vk::UniqueDeviceMemory mSharedMemory;
    vk::UniqueBuffer mSharedBuffer;
};
