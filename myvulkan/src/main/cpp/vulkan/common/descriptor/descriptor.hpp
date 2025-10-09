#pragma once

#include "vulkan/vulkan.hpp"

class Descriptor {
public:
    struct Config {
        uint32_t binding = 0;
        vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eVertex;
        uint32_t maxSets = 1;
    };

    Descriptor(vk::Device device, const Config& config);
    ~Descriptor();

    vk::DescriptorSet allocateDescriptorSet(vk::Device device, vk::Buffer uboBuffer, vk::DeviceSize bufferSize);

private:
    void createDescriptorSetLayout(vk::Device device, const Config& config);
    void createDescriptorPool(vk::Device device, const Config& config);

private:
    vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    vk::UniqueDescriptorPool mDescriptorPool;
    Config mConfig;
};
