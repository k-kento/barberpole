#pragma once

#include <vulkan/vulkan.hpp>

class KaleidoscopeDescriptor {
public:
    struct Config {
        uint32_t maxSets = 0;
        uint32_t binding = 0;
        vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eVertex;
    };

    explicit KaleidoscopeDescriptor(vk::Device &device, Config &config)
            : mDevice(device), mConfig(config) {

        createDescriptorSetLayout();
        createDescriptorPool();
    }

    // descriptor set を作成し、バッファを設定する
    vk::UniqueDescriptorSet allocate(vk::Buffer uboBuffer, vk::DeviceSize bufferSize) {
        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = mDescriptorPool.get();
        allocInfo.descriptorSetCount = 1;
        auto layout = mDescriptorSetLayout.get();
        allocInfo.pSetLayouts = &layout;

        auto sets = mDevice.allocateDescriptorSetsUnique(allocInfo);
        vk::UniqueDescriptorSet descriptorSet = std::move(sets[0]);

        // GPU に DescriptorSet の内容を更新させる
        vk::DescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;

        vk::WriteDescriptorSet write{};
        write.dstSet = descriptorSet.get();
        write.dstBinding = mConfig.binding;
        write.dstArrayElement = 0;
        write.descriptorType = vk::DescriptorType::eUniformBuffer;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;

        mDevice.updateDescriptorSets(1, &write, 0, nullptr);

        return descriptorSet;
    }

    [[nodiscard]] vk::DescriptorSetLayout getLayout() const {
        return mDescriptorSetLayout.get();
    }

private:
    vk::Device &mDevice;
    Config &mConfig;

    vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    vk::UniqueDescriptorPool mDescriptorPool;

    // Shader で使うリソースの「型と場所」を Vulkan に教える
    void createDescriptorSetLayout() {
        vk::DescriptorSetLayoutBinding binding{};
        binding.binding = mConfig.binding;
        binding.descriptorCount = 1;
        binding.descriptorType = vk::DescriptorType::eUniformBuffer;
        binding.stageFlags = mConfig.stageFlags;

        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &binding;

        mDescriptorSetLayout = mDevice.createDescriptorSetLayoutUnique(layoutInfo);
    }

    // DescriptorSet を作るためのプールを作成
    void createDescriptorPool() {
        vk::DescriptorPoolSize poolSize{};
        poolSize.type = vk::DescriptorType::eUniformBuffer;
        poolSize.descriptorCount = mConfig.maxSets;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = mConfig.maxSets;

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }
};