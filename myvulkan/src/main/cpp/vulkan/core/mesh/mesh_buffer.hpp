#pragma once

#include "vulkan_context.h"
#include "vulkan/vulkan.hpp"
#include "device_buffer.h"

template <typename VertexType>

class MeshBuffer {

public:
    MeshBuffer(VulkanContext &context, const std::vector<VertexType> &vertices, const std::vector<uint16_t> &indices) {
        auto device = context.getDevice();

        // Vertex buffer
        VkDeviceSize vboSize = sizeof(vertices[0]) * vertices.size();
        mVertexBuffer = std::make_unique<DeviceBuffer>(
                context,
                vboSize,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
        mVertexBuffer->copyFrom(vertices.data(), vboSize);

        // Index buffer
        mIndexCount = static_cast<uint32_t>(indices.size());
        VkDeviceSize iboSize = sizeof(indices[0]) * mIndexCount;
        mIndexBuffer = std::make_unique<DeviceBuffer>(
                context,
                iboSize,
                vk::BufferUsageFlagBits::eIndexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
        mIndexBuffer->copyFrom(indices.data(), iboSize);
    }

    void bind(vk::CommandBuffer &cmd, uint32_t binding) const {
        vk::Buffer vb = mVertexBuffer->getBuffer();
        vk::DeviceSize offsets[] = {0};
        cmd.bindVertexBuffers(binding, vb, offsets);
        cmd.bindIndexBuffer(mIndexBuffer->getBuffer(), 0, vk::IndexType::eUint16);
    }

    void draw(vk::CommandBuffer &cmd, uint32_t instanceCount) const {
        cmd.drawIndexed(mIndexCount, instanceCount, 0, 0, 0);
    }

private:
    std::unique_ptr<DeviceBuffer> mVertexBuffer;
    std::unique_ptr<DeviceBuffer> mIndexBuffer;
    uint32_t mIndexCount{0};
};
