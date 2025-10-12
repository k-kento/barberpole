#pragma once

#include "vulkan/vulkan.hpp"
#include <cstdint>

class VulkanBuffer {
public:
    VulkanBuffer(vk::Device device,
                 vk::PhysicalDevice physicalDevice,
                 vk::DeviceSize size,
                 vk::BufferUsageFlags usage,
                 vk::MemoryPropertyFlags properties);

    // コピー禁止
    VulkanBuffer(const VulkanBuffer &) = delete;

    VulkanBuffer &operator=(const VulkanBuffer &) = delete;

    // ムーブ可能
    VulkanBuffer(VulkanBuffer &&) noexcept;

    VulkanBuffer &operator=(VulkanBuffer &&) noexcept;

    void copyFrom(const void *data, vk::DeviceSize size, vk::DeviceSize offset = 0);

    vk::Buffer getBuffer() const { return *mBuffer; }

    vk::DeviceMemory getMemory() const { return mMemory.get(); }

private:
    vk::Device mDevice;
    vk::UniqueBuffer mBuffer;
    vk::UniqueDeviceMemory mMemory;
    vk::DeviceSize mSize;
};