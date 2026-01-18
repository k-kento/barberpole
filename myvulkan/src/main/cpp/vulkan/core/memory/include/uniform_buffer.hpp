#pragma once

#include "base_buffer.hpp"
#include "vulkan_context.h"

template <typename T>
class UniformBuffer {
   public:
    explicit UniformBuffer(VulkanContext& context) {
        mBuffer = std::make_unique<BaseBuffer>(
            context,
            sizeof(T),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    void update(const T& data) { mBuffer->copyFrom(&data, sizeof(T)); }

    vk::Buffer getBuffer() const { return mBuffer->getBuffer(); }

    vk::DeviceSize getSize() const { return sizeof(T); }

   private:
    std::unique_ptr<BaseBuffer> mBuffer;
};
