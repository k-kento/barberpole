#pragma once

#include "ubo_buffer.hpp"
#include "vulkan_context.h"
#include <memory>
#include "kaleidoscope_descriptor.hpp"
#include "texture.hpp"

class KaleidoscopeUbo {
public:
    struct UboData {
        glm::mat4 projection;
        glm::mat4 uvMatrix;
    };

    explicit KaleidoscopeUbo(VulkanContext &context, Texture &texture, uint32_t maxFramesInFlight)
            : mContext(context), mMaxFramesInFlight(maxFramesInFlight) {
        auto device = mContext.getDevice();
        mDescriptor = std::make_unique<KaleidoscopeDescriptor>(device);

        mUboBuffers.reserve(mMaxFramesInFlight);
        for (uint32_t i = 0; i < mMaxFramesInFlight; ++i) {
            auto ubo = std::make_unique<UboBuffer<UboData>>(mContext);
            mUboBuffers.push_back(std::move(ubo));
        }

        createDescriptorSets(texture);
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

    void setTexture(Texture &newTexture) {
        createDescriptorSets(newTexture);
    }

private:
    VulkanContext &mContext;
    uint32_t mMaxFramesInFlight;

    std::vector<std::unique_ptr<UboBuffer<UboData>>> mUboBuffers;
    std::unique_ptr<KaleidoscopeDescriptor> mDescriptor;
    std::vector<vk::UniqueDescriptorSet> mDescriptorSets;

    // DescriptorSet を生成（初回／再作成 共通）
    void createDescriptorSets(Texture &texture) {
        mDescriptorSets.clear();
        mDescriptorSets.reserve(mMaxFramesInFlight);

        for (uint32_t i = 0; i < mMaxFramesInFlight; ++i) {
            auto set = mDescriptor->allocate(
                    mUboBuffers[i]->getBuffer(),
                    sizeof(UboData),
                    texture.getImageView(),
                    texture.getSampler()
            );
            mDescriptorSets.push_back(std::move(set));
        }
    }
};
