#pragma once

#include <vulkan/vulkan.hpp>
#include "kaleidoscope_config.hpp"
#include "log.h"

class KaleidoscopeDescriptor {
public:
    explicit KaleidoscopeDescriptor(vk::Device device) : mDevice(device) {
        createDescriptorSetLayout();
        createDescriptorPool();
    }

    vk::UniqueDescriptorSet allocate(vk::Buffer uboBuffer,
                                     vk::DeviceSize uboSize,
                                     vk::ImageView imageView,
                                     vk::Sampler sampler) {

        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = *mDescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &*mDescriptorSetLayout;

        vk::UniqueDescriptorSet descriptorSet;

        try {
            descriptorSet = std::move(mDevice.allocateDescriptorSetsUnique(allocInfo).front());
        } catch (const vk::SystemError &err) {
            throw std::runtime_error("Failed to allocate descriptor set: " + std::string(err.what()));
        }

        vk::DescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = uboSize;

        vk::DescriptorImageInfo imageInfo{};
        imageInfo.imageView = imageView;
        imageInfo.sampler = sampler;
        imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0] = vk::WriteDescriptorSet(
                *descriptorSet,
                0,  // binding
                0,  // array element
                1,  // descriptor count
                vk::DescriptorType::eUniformBuffer,
                nullptr, // pImageInfo
                &bufferInfo,
                nullptr
        );

        descriptorWrites[1] = vk::WriteDescriptorSet(
                *descriptorSet,
                1,
                0,
                1,
                vk::DescriptorType::eCombinedImageSampler,
                &imageInfo,
                nullptr,
                nullptr
        );

        mDevice.updateDescriptorSets(descriptorWrites, nullptr);

        return descriptorSet;
    }

    [[nodiscard]] vk::DescriptorSetLayout getLayout() const {
        return mDescriptorSetLayout.get();
    }

private:
    vk::Device mDevice;
    vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    vk::UniqueDescriptorPool mDescriptorPool;

    // Shader で使うリソースの「型と場所」を Vulkan に教える
    void createDescriptorSetLayout() {
        std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {};

        // binding 0 → UBO
        bindings[0] = vk::DescriptorSetLayoutBinding(
                0,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eVertex,
                nullptr
        );

        // binding 1 → sampler2D
        bindings[1] = vk::DescriptorSetLayoutBinding(
                1,
                vk::DescriptorType::eCombinedImageSampler,
                1,
                vk::ShaderStageFlagBits::eFragment,
                nullptr
        );

        vk::DescriptorSetLayoutCreateInfo layoutInfo{{}, bindings};
        mDescriptorSetLayout = mDevice.createDescriptorSetLayoutUnique(layoutInfo);
    }

    // DescriptorSet を作るためのプールを作成
    void createDescriptorPool() {

        uint32_t maxFramesInFlight = MAX_FRAMES_IN_FLIGHT;

        std::array<vk::DescriptorPoolSize, 2> poolSizes{};

        // 1. UBO
        poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
        poolSizes[0].descriptorCount = maxFramesInFlight;

        // 2. テクスチャサンプラー
        poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
        poolSizes[1].descriptorCount = maxFramesInFlight;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.maxSets = maxFramesInFlight * 2; // DescriptorSet 作り直し分も考慮
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }
};
