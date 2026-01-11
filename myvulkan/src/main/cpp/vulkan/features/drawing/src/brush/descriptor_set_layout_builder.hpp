#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

class DescriptorSetLayoutBuilder {
public:
    DescriptorSetLayoutBuilder& add(uint32_t binding,
                                    vk::DescriptorType type,
                                    vk::ShaderStageFlags stages,
                                    uint32_t count = 1) {
        vk::DescriptorSetLayoutBinding b{};
        b.binding = binding;
        b.descriptorType = type;
        b.descriptorCount = count;
        b.stageFlags = stages;
        mBindings.push_back(b);
        return *this;
    }

    vk::UniqueDescriptorSetLayout build(vk::Device device) const {
        vk::DescriptorSetLayoutCreateInfo info{};
        info.bindingCount = static_cast<uint32_t>(mBindings.size());
        info.pBindings    = mBindings.data();
        return device.createDescriptorSetLayoutUnique(info);
    }

private:
    std::vector<vk::DescriptorSetLayoutBinding> mBindings;
};
