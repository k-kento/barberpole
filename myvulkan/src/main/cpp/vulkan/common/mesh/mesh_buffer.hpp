#pragma once

#include "vulkan_context.h"
#include "vulkan/vulkan.hpp"
#include "device_buffer.h"
#include "vulkan_context.h"
#include "mesh_data.hpp"

class MeshBuffer {

public:
    MeshBuffer(VulkanContext &context, const MeshData &mesh) {
        auto device = context.getVkDevice();

        // Vertex buffer
        VkDeviceSize vboSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();
        mVertexBuffer = std::make_unique<DeviceBuffer>(
                context,
                vboSize,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
        mVertexBuffer->copyFrom(mesh.vertices.data(), vboSize);

        // Index buffer
        mIndexCount = static_cast<uint32_t>(mesh.indices.size());
        VkDeviceSize iboSize = sizeof(mesh.indices[0]) * mIndexCount;
        mIndexBuffer = std::make_unique<DeviceBuffer>(
                context,
                iboSize,
                vk::BufferUsageFlagBits::eIndexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
        mIndexBuffer->copyFrom(mesh.indices.data(), iboSize);
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
