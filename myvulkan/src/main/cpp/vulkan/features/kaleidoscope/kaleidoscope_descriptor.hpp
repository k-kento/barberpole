#pragma once

#include <vulkan/vulkan.hpp>

class KaleidoscopeDescriptor {
public:
    explicit KaleidoscopeDescriptor(vk::Device &device) : mDevice(device) {
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

        auto sets = mDevice.allocateDescriptorSetsUnique(allocInfo);
        auto descriptorSet = std::move(sets[0]);

        std::vector<vk::WriteDescriptorSet> writes;

        // UBO
        vk::DescriptorBufferInfo bufferInfo{uboBuffer, 0, uboSize};
        vk::WriteDescriptorSet uboWrite{};
        uboWrite.dstSet = descriptorSet.get();
        uboWrite.dstBinding = 0;
        uboWrite.dstArrayElement = 0;
        uboWrite.descriptorCount = 1;
        uboWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
        uboWrite.pBufferInfo = &bufferInfo;
        writes.push_back(uboWrite);

        // Image Sampler
        if (imageView && sampler) {
            vk::DescriptorImageInfo imageInfo{sampler, imageView, vk::ImageLayout::eShaderReadOnlyOptimal};
            vk::WriteDescriptorSet imageWrite{};
            imageWrite.dstSet = descriptorSet.get();
            imageWrite.dstBinding = 1;
            imageWrite.dstArrayElement = 0;
            imageWrite.descriptorCount = 1;
            imageWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
            imageWrite.pImageInfo = &imageInfo;
            writes.push_back(imageWrite);
        }

        mDevice.updateDescriptorSets(writes, {});
        return descriptorSet;
    }

    [[nodiscard]] vk::DescriptorSetLayout getLayout() const {
        return mDescriptorSetLayout.get();
    }

private:
    vk::Device &mDevice;
    vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    vk::UniqueDescriptorPool mDescriptorPool;

    // Shader で使うリソースの「型と場所」を Vulkan に教える
    void createDescriptorSetLayout() {
        std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {};

        // binding 0 → UBO
        bindings[0].binding = 0;
        bindings[0].descriptorType = vk::DescriptorType::eUniformBuffer;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;

        // binding 1 → sampler2D
        bindings[1].binding = 1;
        bindings[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags = vk::ShaderStageFlagBits::eFragment;

        vk::DescriptorSetLayoutCreateInfo layoutInfo{{}, bindings};
        mDescriptorSetLayout = mDevice.createDescriptorSetLayoutUnique(layoutInfo);
    }

    // DescriptorSet を作るためのプールを作成
    void createDescriptorPool() {
        auto maxFramesInFlight = 1;

        std::array<vk::DescriptorPoolSize, 2> poolSizes{};

        // 1. UBO
        poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
        poolSizes[0].descriptorCount = 1 * maxFramesInFlight;

        // 2. テクスチャサンプラー用
        poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
        poolSizes[1].descriptorCount = 1 * maxFramesInFlight;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.maxSets = maxFramesInFlight;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }
};
