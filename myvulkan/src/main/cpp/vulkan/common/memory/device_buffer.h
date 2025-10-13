#pragma once

#include "vulkan/vulkan.hpp"
#include <cstdint>

class DeviceBuffer {
public:
    DeviceBuffer(vk::Device device,
                 vk::PhysicalDevice physicalDevice,
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
    vk::Device mDevice;
    vk::UniqueBuffer mBuffer;
    vk::UniqueDeviceMemory mMemory;
    vk::DeviceSize mSize;
};
