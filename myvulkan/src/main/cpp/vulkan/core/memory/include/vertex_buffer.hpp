#pragma once

#include "base_buffer.hpp"
#include "vulkan/vulkan.hpp"

template <typename T>
class VertexBuffer {
   public:
    VertexBuffer(VulkanContext& context, uint32_t maxCount) : mMaxCount(maxCount) {
        VkDeviceSize size = sizeof(T) * maxCount;
        mBuffer = std::make_unique<BaseBuffer>(
            context,
            size,
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    [[nodiscard]] vk::Buffer getRawBuffer() const { return mBuffer->getBuffer(); }

    void update(const T* data, uint32_t count, uint32_t offset = 0) {
        if (!data || count == 0) return;

        if (offset + count > mMaxCount)
            throw std::runtime_error("VertexBuffer::update out of range");

        mBuffer->copyFrom(
            data,
            sizeof(T) * count,
            sizeof(T) * offset);

        mCurrentCount = std::max(mCurrentCount, offset + count);
    }

   private:
    std::unique_ptr<BaseBuffer> mBuffer;
    uint32_t mMaxCount = 0;
    uint32_t mCurrentCount = 0;
};
