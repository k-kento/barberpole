#pragma once

#include "device_buffer.h"
#include "vulkan_context.h"
#include "input_vertex.hpp"

class InputBuffer {
public:
    InputBuffer(VulkanContext &context, size_t maxCount = 100)
            : mContext(context), mMaxCount(maxCount) {
        auto device = context.getDevice();
        mBufferSize = sizeof(InputVertex) * maxCount;

        mDeviceBuffer = std::make_unique<DeviceBuffer>(
                context,
                mBufferSize,
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
    }

    void update(const std::vector<InputVertex>& data) {
        if (data.empty()) return;
        if (data.size() > mMaxCount) {
            throw std::runtime_error("InputBuffer::update() overflow: data too large");
        }
        mDeviceBuffer->copyFrom(data.data(), sizeof(InputVertex) * data.size());
    }

    [[nodiscard]] vk::Buffer getBuffer() const {
        return mDeviceBuffer->getBuffer();
    }

    [[nodiscard]] size_t getMaxCount() const { return mMaxCount; }

    void debugDump(VulkanContext &vkContext, uint32_t count) const {
        auto device = vkContext.getDevice();
        device.waitIdle();  // GPU完了待ち（重要）

        void* mapped = device.mapMemory(mDeviceBuffer->getMemory(), 0, VK_WHOLE_SIZE);

        const auto* vertices = reinterpret_cast<const InputVertex*>(mapped);
        for (uint32_t i = 0; i < count; ++i) {
            LOGD("input[%u] = (%f, %f)", i, vertices[i].position.x, vertices[i].position.y);
        }

        device.unmapMemory(mDeviceBuffer->getMemory());
    }

private:
    VulkanContext &mContext;
    std::unique_ptr<DeviceBuffer> mDeviceBuffer;
    size_t mBufferSize{};
    size_t mMaxCount{};
};
