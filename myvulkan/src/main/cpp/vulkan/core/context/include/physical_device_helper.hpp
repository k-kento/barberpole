#pragma once

#include "vulkan/vulkan.hpp"

struct PhysicalDeviceBundle {
    vk::PhysicalDevice physicalDevice;
    uint32_t queueFamilyIndex = UINT32_MAX;
};

class PhysicalDeviceHelper {
   public:
    PhysicalDeviceHelper() = delete;

    static PhysicalDeviceBundle pickPhysicalDevice(const vk::Instance& instance);

    static uint32_t findMemoryType(const vk::PhysicalDevice& physicalDevice,
                                   uint32_t typeFilter,
                                   vk::MemoryPropertyFlags properties);
};
