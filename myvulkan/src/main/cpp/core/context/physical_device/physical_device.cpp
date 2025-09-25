#include <vulkan/vulkan.h>
#include <vector>
#include "physical_device.h"
#include "log.h"

bool PhysicalDevice::init(VkInstance vkInstance) {
    if (!pickPhysicalDevice(vkInstance)) return false;
    LOGI("PhysicalDevice initialized");
    return true;
}

// Vulkan インスタンスに接続されている GPU（物理デバイス）のうち、アプリケーションで使うデバイスを選択
bool PhysicalDevice::pickPhysicalDevice(VkInstance vkInstance) {
    uint32_t gpuCount = 0;
    auto result = vkEnumeratePhysicalDevices(vkInstance, &gpuCount, nullptr);
    if (result != VK_SUCCESS) {
        LOGE("PhysicalDevice: vkEnumeratePhysicalDevices failed");
        return false;
    }
    if (gpuCount == 0) {
        LOGE("PhysicalDevice: No GPU found");
        return false;
    }

    std::vector<VkPhysicalDevice> gpus(gpuCount);
    result = vkEnumeratePhysicalDevices(vkInstance, &gpuCount, gpus.data());
    if (result != VK_SUCCESS) {
        LOGE("PhysicalDevice: vkEnumeratePhysicalDevices failed");
        return false;
    }

    // ここで最初にグラフィックスキューを持つデバイスを選択
    for (auto gpu : gpus) {
        uint32_t qIndex = PhysicalDevice::findGraphicsQueueFamily(gpu);
        if (qIndex != UINT32_MAX) {
            mPhysicalDevice = gpu;
            mQueueFamilyIndex = qIndex;
            LOGI("PhysicalDevice: GPU selected with graphics queue family %u", qIndex);
            return true;
        }
    }

    LOGE("PhysicalDevice: No suitable GPU with graphics queue found");
    return false;
}

// 特定の物理デバイスで「グラフィックスコマンドを処理できるキューファミリー」を探す
uint32_t PhysicalDevice::findGraphicsQueueFamily(VkPhysicalDevice device) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    if (count == 0) return UINT32_MAX;

    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());
    for (uint32_t i = 0; i < count; ++i) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) return i;
    }
    return UINT32_MAX;
}

