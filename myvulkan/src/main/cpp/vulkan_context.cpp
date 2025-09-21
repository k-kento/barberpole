#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan_context.h"
#include "log.h"
#include <vulkan/vulkan_android.h>
#include <iostream>

// public

VulkanContext::~VulkanContext() {
    destroy();
}

bool VulkanContext::init() {
    if (!createVkInstance()) return false;
    if (!pickPhysicalDevice()) return false;
    if (!createDevice()) return false;

    return true;
}

void VulkanContext::destroy() {
    if (mDevice != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(mDevice);
        vkDestroyDevice(mDevice, nullptr);
    }
    if (mInstance != VK_NULL_HANDLE) vkDestroyInstance(mInstance, nullptr);
    mDevice = VK_NULL_HANDLE;
    mInstance = VK_NULL_HANDLE;

    LOGI("VulkanContext destroyed");
}

VkInstance VulkanContext::getVkInstance() {
    return mInstance;
}

VkPhysicalDevice VulkanContext::getPhysicalDevice() {
    return mPhysicalDevice;
}

VkDevice& VulkanContext::getDevice() {
    return mDevice;
}

uint32_t VulkanContext::getQueueFamilyIndex() {
    return mQueueFamilyIndex;
}

VkQueue VulkanContext::getGraphicsQueue() {
    return mGraphicsQueue;
}

// private

bool VulkanContext::createVkInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "BarberPole";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "NoEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    const char *extensions[] = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
    };

    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = 2;
    instInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateInstance(&instInfo, nullptr, &mInstance) != VK_SUCCESS) {
        LOGE("Failed to create Vulkan instance");
        return false;
    }
    return true;
}

bool VulkanContext::pickPhysicalDevice() {
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &gpuCount, nullptr);
    if (gpuCount == 0) {
        LOGE("No GPU found");
        return false;
    }
    std::vector<VkPhysicalDevice> gpus(gpuCount);
    vkEnumeratePhysicalDevices(mInstance, &gpuCount, gpus.data());
    mPhysicalDevice = gpus[0];
    mQueueFamilyIndex = findGraphicsQueueFamily(mPhysicalDevice);
    if (mQueueFamilyIndex == UINT32_MAX) {
        LOGE("No graphics queue family");
        return false;
    }
    return true;
}

uint32_t VulkanContext::findGraphicsQueueFamily(VkPhysicalDevice device) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());
    for (uint32_t i = 0; i < count; ++i) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) return i;
    }
    return UINT32_MAX;
}

bool VulkanContext::createDevice() {
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = mQueueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    const char *deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(mPhysicalDevice, &deviceInfo, nullptr, &mDevice) != VK_SUCCESS) {
        LOGE("Failed to create logical device");
        return false;
    }

    vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &mGraphicsQueue);
    return true;
}
