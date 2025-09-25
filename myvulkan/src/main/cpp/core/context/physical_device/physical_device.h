#pragma once

#include <vulkan/vulkan.h>

class PhysicalDevice {

public:
    PhysicalDevice() = default;

    ~PhysicalDevice() = default;

    bool init(VkInstance vkInstance);

    [[nodiscard]] uint32_t getQueueFamilyIndex() const { return mQueueFamilyIndex; }

    [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const { return mPhysicalDevice; }

private:
    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    uint32_t mQueueFamilyIndex = UINT32_MAX;

    bool pickPhysicalDevice(VkInstance vkInstance);

    static uint32_t findGraphicsQueueFamily(VkPhysicalDevice device);

};
