#pragma once

#include "vulkan/vulkan.hpp"
#include "log.h"

class Descriptor {
public:
    explicit Descriptor(vk::Device device) : mDevice(device) {
        createDescriptorSetLayout();
        createDescriptorPool();
    }

    vk::UniqueDescriptorSet allocate(vk::Buffer uboBuffer,
                                     vk::DeviceSize uboSize
    ) {

        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = *mDescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &*mDescriptorSetLayout;

        vk::UniqueDescriptorSet descriptorSet;

        try {
            auto sets = mDevice.allocateDescriptorSetsUnique(allocInfo);
            descriptorSet = std::move(sets.front());
        } catch (const vk::SystemError &err) {
            throw std::runtime_error("Failed to allocate descriptor set: " + std::string(err.what()));
        }

        vk::DescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = uboSize;

        std::array<vk::WriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0] = vk::WriteDescriptorSet(
                *descriptorSet,
                0,
                0,
                1,
                vk::DescriptorType::eUniformBuffer,
                nullptr,
                &bufferInfo,
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
        std::array<vk::DescriptorSetLayoutBinding, 1> bindings = {};

        // binding 0 → UBO
        bindings[0] = vk::DescriptorSetLayoutBinding(
                0,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eVertex,
                nullptr
        );

        vk::DescriptorSetLayoutCreateInfo layoutInfo{{}, bindings};
        mDescriptorSetLayout = mDevice.createDescriptorSetLayoutUnique(layoutInfo);
    }

    // DescriptorSet を作るためのプールを作成
    void createDescriptorPool() {

        uint32_t maxFramesInFlight = SurfaceContext::MAX_FRAMES_IN_FLIGHT;

        std::array<vk::DescriptorPoolSize, 1> poolSizes{};

        // UBO
        poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
        poolSizes[0].descriptorCount = maxFramesInFlight;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.maxSets = maxFramesInFlight;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }
};
