#pragma once

#include "device_buffer.h"
#include "vulkan_context.h"

template <typename T>
class UboBuffer {
   public:
    explicit UboBuffer(VulkanContext& context) {
        mBuffer = std::make_unique<DeviceBuffer>(
            context,
            sizeof(T),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    void update(const T& data) { mBuffer->copyFrom(&data, sizeof(T)); }

    vk::Buffer getBuffer() const { return mBuffer->getBuffer(); }

    vk::DeviceSize getSize() const { return sizeof(T); }

   private:
    std::unique_ptr<DeviceBuffer> mBuffer;
};
