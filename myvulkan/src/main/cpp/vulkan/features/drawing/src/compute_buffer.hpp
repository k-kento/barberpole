#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "input_vertex.hpp"
#include "log.h"

class ComputeBuffer {

public:

    ComputeBuffer(VulkanContext &vkContext) {
        auto device = vkContext.getDevice();

        uint32_t MAX_INPUT_POINTS = 1000; // TODO
        const uint32_t MAX_OUTPUT_VERTICES = (MAX_INPUT_POINTS - 1) * 6; // TODO

        vk::BufferCreateInfo bufferInfo{};
        bufferInfo.size = sizeof(InputVertex) * MAX_OUTPUT_VERTICES;
        bufferInfo.usage = vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        mSharedBuffer = device.createBufferUnique(bufferInfo);
        vk::MemoryRequirements memReq = device.getBufferMemoryRequirements(mSharedBuffer.get());

        auto physicalDevice = vkContext.getPhysicalDevice();

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memReq.size;
        allocInfo.memoryTypeIndex = PhysicalDeviceHelper::findMemoryType(
                physicalDevice,
                memReq.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        mSharedMemory = device.allocateMemoryUnique(allocInfo);
        device.bindBufferMemory(mSharedBuffer.get(), mSharedMemory.get(), 0);
    }

    [[nodiscard]] vk::Buffer getBuffer() const {
        return mSharedBuffer.get();
    }

    void debugDump(VulkanContext &vkContext, uint32_t count) const {
        vkContext.getDevice().waitIdle();
        auto device = vkContext.getDevice();
        void* mapped = device.mapMemory(mSharedMemory.get(), 0, VK_WHOLE_SIZE);

        auto* vertices = reinterpret_cast<const InputVertex*>(mapped);

        for (uint32_t i = 0; i < count; ++i) {
            LOGD("output %f, %f", vertices[i].position.x, vertices[i].position.y);
        }

        device.unmapMemory(mSharedMemory.get());
    }

private:
    vk::UniqueDeviceMemory mSharedMemory;
    vk::UniqueBuffer mSharedBuffer;
};
