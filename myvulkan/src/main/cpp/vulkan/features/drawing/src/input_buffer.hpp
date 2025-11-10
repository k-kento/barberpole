#pragma once

#include "device_buffer.h"
#include "vulkan_context.h"
#include "input_vertex.hpp"
#include "renderer_constants.hpp"

class InputBuffer {
public:
    InputBuffer(VulkanContext &context) {
        auto device = context.getDevice();
        mBufferSize = sizeof(InputVertex) * MAX_INPUT_POINTS;

        mDeviceBuffer = std::make_unique<DeviceBuffer>(
                context,
                mBufferSize,
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
    }

    void update(const InputVertex* data, size_t count, size_t dstVertexOffset) {
        if (count == 0) return;
        LOGI("InputBuffer update: count = %zu", count);
        const size_t byteSize = sizeof(InputVertex) * count;
        const size_t dstBytes = sizeof(InputVertex) * dstVertexOffset;
        if (dstVertexOffset + count > MAX_INPUT_POINTS)
            throw std::runtime_error("InputBuffer::updateRange overflow");
        mDeviceBuffer->copyFrom(data, byteSize, dstBytes);
    }

    [[nodiscard]] vk::Buffer getBuffer() const {
        return mDeviceBuffer->getBuffer();
    }

private:
    std::unique_ptr<DeviceBuffer> mDeviceBuffer;
    size_t mBufferSize{};
};
