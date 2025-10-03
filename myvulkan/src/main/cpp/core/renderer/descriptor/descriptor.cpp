#pragma once

#include "descriptor.hpp"
#include <stdexcept>
#include "log.h"

Descriptor::Descriptor(vk::Device device, const Config &config) : mConfig(config) {
    createDescriptorSetLayout(device, config);
    createDescriptorPool(device, config);
}

Descriptor::~Descriptor() {
    LOGI("Descriptor destroyed.");
}

void Descriptor::createDescriptorSetLayout(vk::Device device, const Config &config) {
    vk::DescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = config.binding;
    uboBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    uboBinding.descriptorCount = 1;
    uboBinding.stageFlags = config.stageFlags;

    vk::DescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboBinding;

    mDescriptorSetLayout = device.createDescriptorSetLayoutUnique(layoutInfo);
}

void Descriptor::createDescriptorPool(vk::Device device, const Config &config) {
    vk::DescriptorPoolSize poolSize{};
    poolSize.type = vk::DescriptorType::eUniformBuffer;
    poolSize.descriptorCount = config.maxSets;

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = config.maxSets;

    mDescriptorPool = device.createDescriptorPoolUnique(poolInfo);
}

vk::DescriptorSet Descriptor::allocateDescriptorSet(vk::Device device, vk::Buffer uboBuffer, vk::DeviceSize bufferSize) {
    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool = mDescriptorPool.get();
    allocInfo.descriptorSetCount = 1;
    auto layout = mDescriptorSetLayout.get();
    allocInfo.pSetLayouts = &layout;

    auto sets = device.allocateDescriptorSets(allocInfo);
    vk::DescriptorSet descriptorSet = sets[0];

    vk::DescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uboBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = bufferSize;

    vk::WriteDescriptorSet write{};
    write.dstSet = descriptorSet;
    write.dstBinding = mConfig.binding;
    write.dstArrayElement = 0;
    write.descriptorType = vk::DescriptorType::eUniformBuffer;
    write.descriptorCount = 1;
    write.pBufferInfo = &bufferInfo;

    device.updateDescriptorSets(1, &write, 0, nullptr);

    return descriptorSet;
}
