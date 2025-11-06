#pragma once

#include "device_buffer.h"
#include "vulkan_context.h"
#include "view_bounds.hpp"
#include <glm/glm.hpp>
#include "surface_context.hpp"

namespace {
    constexpr uint32_t MAX_INSTANCE_COUNT = 10000;
}

struct InstanceData {
    glm::mat4 model;
};

class InstanceBuffer {

public:

    explicit InstanceBuffer(VulkanContext &context) {
        VkDeviceSize instanceBufferSize = sizeof(InstanceData) * MAX_INSTANCE_COUNT;

        mDeviceBuffer = std::make_unique<DeviceBuffer>(
                context,
                instanceBufferSize,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
        mMappedPtr = static_cast<InstanceData *>(mDeviceBuffer->map());
    }

    [[nodiscard]] uint32_t getInstanceCount() const {
        return MAX_INSTANCE_COUNT;
    }

    [[nodiscard]] vk::Buffer getDeviceBuffer() const {
        return mDeviceBuffer->getBuffer();
    }

    void updateInstances(const std::vector<InstanceData> &newInstances) {
        VkDeviceSize fullSize = sizeof(InstanceData) * MAX_INSTANCE_COUNT;
        std::memset(mMappedPtr, 0, static_cast<size_t>(fullSize));

        if (newInstances.empty()) return;

        VkDeviceSize copySize = sizeof(InstanceData) * newInstances.size();
        if (copySize > MAX_INSTANCE_COUNT * sizeof(InstanceData))
            throw std::runtime_error("Too many instances for buffer");

        std::memcpy(mMappedPtr, newInstances.data(), static_cast<size_t>(copySize));

        mInstanceCounts = static_cast<uint32_t>(newInstances.size());
    }

private:
    uint32_t mInstanceCounts{};
    InstanceData * mMappedPtr;
    std::unique_ptr<DeviceBuffer> mDeviceBuffer;

};
