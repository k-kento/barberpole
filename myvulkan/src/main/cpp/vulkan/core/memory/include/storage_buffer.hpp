#pragma once

#include <cstring>
#include <stdexcept>
#include <vector>

#include "base_buffer.hpp"

template <typename T>
class StorageBuffer {
   public:
    StorageBuffer(VulkanContext& context, size_t count) : mCount(count) {
        mBufferSize = sizeof(T) * count;
        mBuffer = std::make_unique<BaseBuffer>(
            context,
            mBufferSize,
            vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    void update(const std::vector<T>& data) {
        if (data.size() > mCount) throw std::runtime_error("StorageBuffer: data exceeds allocated count");
        mBuffer->copyFrom(data.data(), sizeof(T) * data.size());
    }

    vk::Buffer getBuffer() const { return mBuffer->getBuffer(); }

    size_t getCount() const { return mCount; }

    size_t getSize() const { return mBufferSize; }

   private:
    size_t mCount;
    size_t mBufferSize;
    std::unique_ptr<BaseBuffer> mBuffer;
};
