#pragma once

#include "device_buffer.h"
#include "mirror_tile_grid.hpp"
#include "vulkan_context.h"

struct InstanceData {
    glm::mat4 model;
};

class KaleidoscopeInstanceBuffer {

public:

    KaleidoscopeInstanceBuffer(VulkanContext &context, const ViewBounds &viewBounds) {

        auto device = context.getDevice();

        auto slices = MirrorTileGrid::createTileGrid(viewBounds);
        mInstanceCount = static_cast<uint32_t>(slices.size());

        std::vector<InstanceData> instances;
        instances.reserve(mInstanceCount);
        for (const auto &slice: slices) {
            instances.push_back({slice});
        }

        VkDeviceSize instanceBufferSize = sizeof(InstanceData) * mInstanceCount;

        mDeviceBuffer = std::make_unique<DeviceBuffer>(
                context,
                instanceBufferSize,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        mDeviceBuffer->copyFrom(instances.data(), instanceBufferSize);
    }

    [[nodiscard]] uint32_t getInstanceCount() const {
        return mInstanceCount;
    }

    [[nodiscard]] vk::Buffer getDeviceBuffer() const {
        return mDeviceBuffer->getBuffer();
    }

private:

    uint32_t mInstanceCount;
    std::unique_ptr<DeviceBuffer> mDeviceBuffer;

};
