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

        for (uint32_t i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
            mDeviceBuffers[i] = std::make_unique<DeviceBuffer>(
                    context,
                    instanceBufferSize,
                    vk::BufferUsageFlagBits::eVertexBuffer,
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
            );
            mMappedPtrs[i] = static_cast<InstanceData *>(mDeviceBuffers[i]->map());
        }
    }

    [[nodiscard]] uint32_t getInstanceCount(uint32_t frameIndex) const {
        return MAX_INSTANCE_COUNT;
    }

    [[nodiscard]] vk::Buffer getDeviceBuffer(uint32_t frameIndex) const {
        return mDeviceBuffers[frameIndex]->getBuffer();
    }

    void updateInstances(uint32_t frameIndex, const std::vector<InstanceData> &newInstances) {
        VkDeviceSize fullSize = sizeof(InstanceData) * MAX_INSTANCE_COUNT;
        std::memset(mMappedPtrs[frameIndex], 0, static_cast<size_t>(fullSize));

        if (newInstances.empty()) return;

        VkDeviceSize copySize = sizeof(InstanceData) * newInstances.size();
        if (copySize > MAX_INSTANCE_COUNT * sizeof(InstanceData))
            throw std::runtime_error("Too many instances for buffer");

        std::memcpy(mMappedPtrs[frameIndex], newInstances.data(), static_cast<size_t>(copySize));

        mInstanceCounts[frameIndex] = static_cast<uint32_t>(newInstances.size());
    }

private:
    std::array<uint32_t, SurfaceContext::MAX_FRAMES_IN_FLIGHT> mInstanceCounts{};
    std::array<InstanceData *, SurfaceContext::MAX_FRAMES_IN_FLIGHT> mMappedPtrs;
    std::array<std::unique_ptr<DeviceBuffer>, SurfaceContext::MAX_FRAMES_IN_FLIGHT> mDeviceBuffers;

};
