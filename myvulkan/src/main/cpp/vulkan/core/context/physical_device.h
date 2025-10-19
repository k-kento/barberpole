#pragma once

#include <vulkan/vulkan.hpp>

class PhysicalDevice {

public:
    PhysicalDevice(const vk::Instance &instance) noexcept(false);

    [[nodiscard]] uint32_t getQueueFamilyIndex() const { return mQueueFamilyIndex; }

    [[nodiscard]] vk::PhysicalDevice getPhysicalDevice() const { return mPhysicalDevice; }

    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

private:
    vk::PhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex = UINT32_MAX;

    bool pickPhysicalDevice(const vk::Instance &instance);

    static uint32_t findGraphicsQueueFamily(vk::PhysicalDevice device);

};
