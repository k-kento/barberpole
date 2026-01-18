#pragma once

#include <stdexcept>
#include <type_traits>
#include <vector>

#include "base_buffer.hpp"

template <typename T>
class InstanceBuffer {
   public:
    static_assert(std::is_trivially_copyable_v<T>, "InstanceBuffer requires POD type");

    InstanceBuffer(VulkanContext& context, size_t maxCount) : mMaxCount(maxCount) {
        mBuffer = std::make_unique<BaseBuffer>(
            context,
            sizeof(T) * maxCount,
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    void update(const std::vector<T>& data) {
        if (data.empty()) return;

        if (data.size() > mMaxCount) throw std::runtime_error("InstanceBuffer: overflow");

        mBuffer->copyFrom(data.data(), sizeof(T) * data.size());

        mCount = static_cast<uint32_t>(data.size());
    }

    vk::Buffer buffer() const { return mBuffer->getBuffer(); }

    uint32_t count() const { return mCount; }

   private:
    size_t mMaxCount{};
    uint32_t mCount{};
    std::unique_ptr<BaseBuffer> mBuffer;
};
