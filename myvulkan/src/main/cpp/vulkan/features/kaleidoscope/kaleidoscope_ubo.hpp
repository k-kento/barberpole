#pragma once

#include "ubo_buffer.hpp"
#include "vulkan_context.h"
#include <memory>
#include "kaleidoscope_descriptor.hpp"
#include "../../common/texture/texture.hpp"

class KaleidoscopeUbo {
public:
    struct UboData {
        glm::mat4 projection;
    };

    explicit KaleidoscopeUbo(VulkanContext &context, Texture& texture) : mContext(context) {
        auto device = mContext.getVkDevice();
        mUboBuffer = std::make_unique<UboBuffer<UboData>>(context);
        mDescriptor = std::make_unique<KaleidoscopeDescriptor>(device);

        mDescriptorSet = mDescriptor->allocate(mUboBuffer->getBuffer(),
                                               sizeof(UboData),
                                               texture.getImageView(),
                                               texture.getSampler());
    }

    void update(const UboData &data) {
        mUboBuffer->update(data);
    }

    [[nodiscard]] vk::DescriptorSet getDescriptorSet() const {
        return mDescriptorSet.get();
    }

    [[nodiscard]] vk::DescriptorSetLayout getDescriptorSetLayout() const {
        return mDescriptor->getLayout();
    }

private:

    VulkanContext &mContext;
    std::unique_ptr<UboBuffer<UboData>> mUboBuffer;
    std::unique_ptr<KaleidoscopeDescriptor> mDescriptor;
    vk::UniqueDescriptorSet mDescriptorSet;
};
