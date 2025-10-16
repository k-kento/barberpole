#pragma once

#include "vulkan/vulkan.hpp"
#include <cstdint>
#include "vulkan_context.h"

class DeviceBuffer {
public:
    DeviceBuffer(VulkanContext &context,
                 vk::DeviceSize size,
                 vk::BufferUsageFlags usage,
                 vk::MemoryPropertyFlags properties);

    // コピー禁止
    DeviceBuffer(const DeviceBuffer &) = delete;

    DeviceBuffer &operator=(const DeviceBuffer &) = delete;

    // ムーブ可能
    DeviceBuffer(DeviceBuffer &&) noexcept;

    DeviceBuffer &operator=(DeviceBuffer &&) noexcept;

    void copyFrom(const void *data, vk::DeviceSize size, vk::DeviceSize offset = 0);

    vk::Buffer getBuffer() const { return *mBuffer; }

    vk::DeviceMemory getMemory() const { return mMemory.get(); }

private:
    VulkanContext &mContext;
    vk::UniqueBuffer mBuffer;
    vk::UniqueDeviceMemory mMemory;
    vk::DeviceSize mSize;
};
