#pragma once

#include <stdexcept>
#include <type_traits>
#include <vector>

#include "base_buffer.hpp"

class IndexBuffer {
   public:
    IndexBuffer(VulkanContext& context, uint32_t maxCount) : mMaxCount(maxCount) {
        VkDeviceSize size = sizeof(uint16_t) * maxCount;
        mBuffer = std::make_unique<BaseBuffer>(
            context,
            size,
            vk::BufferUsageFlagBits::eIndexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    [[nodiscard]] vk::Buffer getRawBuffer() const { return mBuffer->getBuffer(); }

    void update(const uint16_t* data, uint32_t count, uint32_t offset = 0) {
        if (!data || count == 0) return;

        if (offset + count > mMaxCount)
            throw std::runtime_error("IndexBuffer::update out of range");

        mBuffer->copyFrom(
            data,
            sizeof(uint16_t) * count,
            sizeof(uint16_t) * offset);

        mCurrentCount = std::max(mCurrentCount, offset + count);
    }

   private:
    std::unique_ptr<BaseBuffer> mBuffer;
    uint32_t mMaxCount = 0;
    uint32_t mCurrentCount = 0;
};
