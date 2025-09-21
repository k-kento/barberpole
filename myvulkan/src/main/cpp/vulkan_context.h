#pragma once

#include <vulkan/vulkan.h>

class VulkanContext {

public:
    ~VulkanContext();

    bool init();

    void destroy();

    VkInstance getVkInstance();

    VkPhysicalDevice getPhysicalDevice();

    VkDevice& getDevice();

    uint32_t getQueueFamilyIndex();

    VkQueue getGraphicsQueue();

private:
    VkInstance mInstance = VK_NULL_HANDLE;
    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    VkDevice mDevice = VK_NULL_HANDLE;
    uint32_t mQueueFamilyIndex = UINT32_MAX;
    VkQueue mGraphicsQueue = VK_NULL_HANDLE;

    bool createVkInstance();

    bool pickPhysicalDevice();

    uint32_t findGraphicsQueueFamily(VkPhysicalDevice device);

    bool createDevice();

};
