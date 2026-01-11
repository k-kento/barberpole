#pragma once
#include "vulkan/vulkan.hpp"
#include "device_buffer.h"
#include <cstring>
#include <stdexcept>

template <typename T>
class GenericBuffer {
public:
    GenericBuffer(VulkanContext& context,
                  size_t maxCount,
                  vk::BufferUsageFlags usage,
                  vk::MemoryPropertyFlags properties)
            : mContext(context), mMaxCount(maxCount) {

        mSize = sizeof(T) * maxCount;

        mDeviceBuffer = std::make_unique<DeviceBuffer>(
                context,
                mSize,
                usage,
                properties
        );
    }

    void update(const T* data, size_t count, size_t offset = 0) {
        if (!data || count == 0) return;
        if (offset + count > mMaxCount)
            throw std::runtime_error("GenericBuffer::update: out of range");

        mDeviceBuffer->copyFrom(data, sizeof(T) * count, sizeof(T) * offset);
        mCurrentCount = static_cast<uint32_t>(std::max<size_t>(mCurrentCount, offset + count));
    }

    vk::Buffer getBuffer() const { return mDeviceBuffer->getBuffer(); }
    vk::DeviceSize getSize() const { return mSize; }
    uint32_t getCount() const { return mCurrentCount; }

protected:
    VulkanContext& mContext;
    std::unique_ptr<DeviceBuffer> mDeviceBuffer;
    vk::DeviceSize mSize = 0;
    uint32_t mCurrentCount = 0;
    size_t mMaxCount;
};
