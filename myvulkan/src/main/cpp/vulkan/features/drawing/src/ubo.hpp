#pragma once

#include "ubo_buffer.hpp"
#include "vulkan_context.h"
#include <memory>
#include "texture.hpp"
#include "glm/glm.hpp"
#include "descriptor.hpp"

class Ubo {
public:
    struct UboData {
        glm::mat4 projection;
    };

    explicit Ubo(VulkanContext &context, uint32_t maxFramesInFlight) :
            mContext(context),
            mMaxFramesInFlight(maxFramesInFlight),
            mDescriptor(std::make_unique<Descriptor>(context.getDevice())) {
        auto device = mContext.getDevice();
        mUboBuffers.reserve(mMaxFramesInFlight);
        for (uint32_t i = 0; i < mMaxFramesInFlight; ++i) {
            auto ubo = std::make_unique<UboBuffer<UboData>>(mContext);
            mUboBuffers.push_back(std::move(ubo));
        }

        createDescriptorSets();
    }

    void update(const UboData &data, uint32_t currentFrame) {
        if (currentFrame >= mMaxFramesInFlight) return;
        mUboBuffers[currentFrame]->update(data);
    }

    vk::DescriptorSet getDescriptorSet(uint32_t currentFrame) const {
        if (currentFrame >= mMaxFramesInFlight) return VK_NULL_HANDLE;
        return mDescriptorSets[currentFrame].get();
    }

    vk::DescriptorSetLayout getDescriptorSetLayout() const {
        return mDescriptor->getLayout();
    }

private:
    VulkanContext &mContext;
    uint32_t mMaxFramesInFlight;

    std::vector<std::unique_ptr<UboBuffer<UboData>>> mUboBuffers;
    std::unique_ptr<Descriptor> mDescriptor;
    std::vector<vk::UniqueDescriptorSet> mDescriptorSets;

    // DescriptorSet を生成（初回／再作成 共通）
    void createDescriptorSets() {
        mDescriptorSets.clear();
        mDescriptorSets.reserve(mMaxFramesInFlight);

        for (uint32_t i = 0; i < mMaxFramesInFlight; ++i) {
            auto set = mDescriptor->allocate(
                    mUboBuffers[i]->getBuffer(),
                    sizeof(UboData)
            );
            mDescriptorSets.push_back(std::move(set));
        }
    }
};
