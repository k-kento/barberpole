#pragma once

#include "ubo_buffer.hpp"
#include "vulkan_context.h"
#include <memory>
#include "kaleidoscope_descriptor.hpp"

class KaleidoscopeUbo {
public:
    struct UboData {
        glm::mat4 projection;
    };

    explicit KaleidoscopeUbo(VulkanContext &context) : mContext(context) {
        auto device = mContext.getVkDevice();
        auto physicalDevice = mContext.getVkPhysicalDevice();
        mUboBuffer = std::make_unique<UboBuffer<UboData>>(device, physicalDevice);

        KaleidoscopeDescriptor::Config config;
        config.binding = 0;
        config.maxSets = 1;
        mDescriptor = std::make_unique<KaleidoscopeDescriptor>(device, config);

        mDescriptorSet = mDescriptor->allocate(mUboBuffer->getBuffer(), sizeof(UboData));
    }

    void update(const UboData &data) {
        mUboBuffer->update(data);
    }

    [[nodiscard]] vk::DescriptorSet getDescriptorSet() const {
        return mDescriptorSet.get();
    }

private:

    VulkanContext &mContext;
    std::unique_ptr<UboBuffer<UboData>> mUboBuffer;
    std::unique_ptr<KaleidoscopeDescriptor> mDescriptor;
    vk::UniqueDescriptorSet mDescriptorSet;
};
