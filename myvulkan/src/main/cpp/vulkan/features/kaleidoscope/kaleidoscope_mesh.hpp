#pragma once

#include "vulkan/vulkan.hpp"
#include "device_buffer.h"
#include "vulkan_context.h"
#include "regular_polygon.hpp"

class KaleidoscopeMesh {

public:

    explicit KaleidoscopeMesh(VulkanContext &context) {
        vk::Device device = context.getVkDevice();
        vk::PhysicalDevice physicalDevice = context.getVkPhysicalDevice();

        mesh = std::make_unique<RegularPolygon>(3, 1.0f);

        /* Vertex Buffer */
        vk::DeviceSize bufferSize = sizeof(mesh->vertices[0]) * mesh->vertices.size();

        vertexBuffer = std::make_unique<DeviceBuffer>(
                device,
                physicalDevice,
                bufferSize,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        vertexBuffer->copyFrom(mesh->vertices.data(), bufferSize);

        /* Index Buffer */
        indexCount = mesh->indices.size();
        bufferSize = sizeof(mesh->indices[0]) * indexCount;

        indexBuffer = std::make_unique<DeviceBuffer>(
                device,
                physicalDevice,
                bufferSize,
                vk::BufferUsageFlagBits::eIndexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        indexBuffer->copyFrom(mesh->indices.data(), bufferSize);
    }

    [[nodiscard]] vk::Buffer getVertexBuffer() const {
        return vertexBuffer->getBuffer();
    }

    [[nodiscard]] vk::Buffer getIndexBuffer() const {
        return indexBuffer->getBuffer();
    }

    [[nodiscard]] uint32_t getIndexCount() const {
        return indexCount;
    }

private:
    uint32_t indexCount{0};
    std::unique_ptr<DeviceBuffer> vertexBuffer;
    std::unique_ptr<DeviceBuffer> indexBuffer;
    std::unique_ptr<RegularPolygon> mesh;
};
