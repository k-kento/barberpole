#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "surface_context.hpp"

class ComputeDescriptor {
   public:
    ComputeDescriptor(vk::Device device) : mDevice(device) {
        createDescriptorSetLayout();
        createDescriptorPool();
    }

    vk::UniqueDescriptorSet allocateDescriptorSet() {
        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = mDescriptorPool.get();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &mDescriptorSetLayout.get();
        auto uniqueSets = mDevice.allocateDescriptorSetsUnique(allocInfo);
        return std::move(uniqueSets[0]);
    }

    // DescriptorSetLayout getter
    vk::DescriptorSetLayout getLayout() const { return mDescriptorSetLayout.get(); }

    // Descriptor 更新（StorageBuffer専用）
    void updateStorageBuffers(vk::DescriptorSet descriptorSet,
                              const vk::Buffer& inputBuffer,
                              vk::DeviceSize inputSize,
                              const vk::Buffer& outputBuffer,
                              vk::DeviceSize outputSize) {
        std::array<vk::WriteDescriptorSet, 2> writes{};

        vk::DescriptorBufferInfo inputInfo{};
        inputInfo.buffer = inputBuffer;
        inputInfo.offset = 0;
        inputInfo.range = inputSize;

        vk::DescriptorBufferInfo outputInfo{};
        outputInfo.buffer = outputBuffer;
        outputInfo.offset = 0;
        outputInfo.range = outputSize;

        writes[0].dstSet = descriptorSet;
        writes[0].dstBinding = 0;
        writes[0].descriptorCount = 1;
        writes[0].descriptorType = vk::DescriptorType::eStorageBuffer;
        writes[0].pBufferInfo = &inputInfo;

        writes[1].dstSet = descriptorSet;
        writes[1].dstBinding = 1;
        writes[1].descriptorCount = 1;
        writes[1].descriptorType = vk::DescriptorType::eStorageBuffer;
        writes[1].pBufferInfo = &outputInfo;

        mDevice.updateDescriptorSets(writes, nullptr);
    }

   private:
    vk::Device mDevice;
    vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    vk::UniqueDescriptorPool mDescriptorPool;

    void createDescriptorSetLayout() {
        // binding 0: input buffer
        vk::DescriptorSetLayoutBinding inputBinding{};
        inputBinding.binding = 0;
        inputBinding.descriptorType = vk::DescriptorType::eStorageBuffer;
        inputBinding.descriptorCount = 1;
        inputBinding.stageFlags = vk::ShaderStageFlagBits::eCompute;

        // binding 1: output buffer
        vk::DescriptorSetLayoutBinding outputBinding{};
        outputBinding.binding = 1;
        outputBinding.descriptorType = vk::DescriptorType::eStorageBuffer;
        outputBinding.descriptorCount = 1;
        outputBinding.stageFlags = vk::ShaderStageFlagBits::eCompute;

        std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {inputBinding, outputBinding};

        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        mDescriptorSetLayout = mDevice.createDescriptorSetLayoutUnique(layoutInfo);
    }

    void createDescriptorPool() {
        constexpr uint32_t BINDINGS_PER_SET = 2;  // input/output buffer
        std::array<vk::DescriptorPoolSize, 1> poolSizes{};
        poolSizes[0].type = vk::DescriptorType::eStorageBuffer;
        poolSizes[0].descriptorCount = BINDINGS_PER_SET * SurfaceContext::MAX_FRAMES_IN_FLIGHT;  // input + output

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = SurfaceContext::MAX_FRAMES_IN_FLIGHT;
        poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }
};
