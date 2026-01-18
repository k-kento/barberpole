#pragma once

#include <cstdint>

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

class BaseBuffer {
   public:
    BaseBuffer(VulkanContext& context,
             vk::DeviceSize size,
             vk::BufferUsageFlags usage,
             vk::MemoryPropertyFlags properties);

    // コピー禁止
    BaseBuffer(const BaseBuffer&) = delete;

    BaseBuffer& operator=(const BaseBuffer&) = delete;

    // ムーブ可能
    BaseBuffer(BaseBuffer&&) noexcept;

    BaseBuffer& operator=(BaseBuffer&&) noexcept;

    void copyFrom(const void* data, vk::DeviceSize size, vk::DeviceSize offset = 0);

    vk::Buffer getBuffer() const { return *mBuffer; }

    vk::DeviceMemory getMemory() const { return mMemory.get(); }

    void* map() { return mContext.getDevice().mapMemory(*mMemory, 0, mSize); }

    void unmap() { mContext.getDevice().unmapMemory(*mMemory); }

   private:
    VulkanContext& mContext;
    vk::UniqueBuffer mBuffer;
    vk::UniqueDeviceMemory mMemory;
    vk::DeviceSize mSize;
};
