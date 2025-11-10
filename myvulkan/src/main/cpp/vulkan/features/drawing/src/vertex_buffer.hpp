#pragma once

#include "device_buffer.h"
#include "vulkan_context.h"
#include "input_vertex.hpp"
#include "renderer_constants.hpp"

class VertexBuffer {
public:
    explicit VertexBuffer(VulkanContext &context)
            : mContext(context) {
        const VkDeviceSize bufferSize = sizeof(InputVertex) * MAX_COMPUTE_OUTPUT_VERTICES;

        mDeviceBuffer = std::make_unique<DeviceBuffer>(
                context,
                bufferSize,
                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        mBufferSize = bufferSize;
    }

    void update(const InputVertex *data, uint32_t count, uint32_t startIndex = 0) {
        VkDeviceSize offset = sizeof(InputVertex) * startIndex;
        VkDeviceSize size = sizeof(InputVertex) * count;
        VkDeviceSize total = offset + size;

        if (total > mBufferSize) {
            LOGE("Buffer overflow prevented: total %zu > buffer %llu", (size_t) total, mBufferSize);
        }

        mDeviceBuffer->copyFrom(data, size, offset);
        mWrittenVertexCount += count;
    }

    [[nodiscard]] vk::Buffer getBuffer() const noexcept {
        return mDeviceBuffer->getBuffer();
    }

    [[nodiscard]] VkDeviceSize getSize() const noexcept {
        return mBufferSize;
    }

    /**
     * 書き込み済みの頂点数を取得
     */
    [[nodiscard]] uint32_t getWrittenVertexCount() const noexcept {
        return mWrittenVertexCount;
    }

private:
    VulkanContext &mContext;
    uint32_t mWrittenVertexCount = 0;
    std::unique_ptr<DeviceBuffer> mDeviceBuffer;
    VkDeviceSize mBufferSize{};
};
