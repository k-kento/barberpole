#include <vulkan/vulkan.hpp>
#include <vector>
#include "physical_device.h"
#include "log.h"
#include <stdexcept>

PhysicalDevice::PhysicalDevice(const vk::Instance &instance) {
    if (!pickPhysicalDevice(instance)) {
        throw std::runtime_error("Failed to pick physical device");
    }
    LOGI("Physical device created.");
}

// 指定したプロパティを満たすメモリタイプのインデックスを返す
uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties memProperties = mPhysicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type.");
}

// 接続されているデバイスの中からグラフィックスコマンドを処理できるものを探す
bool PhysicalDevice::pickPhysicalDevice(const vk::Instance &instance) {
    std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
    if (devices.empty()) {
        LOGE("PhysicalDevice: No GPU found");
        return false;
    }

    for (auto device: devices) {
        uint32_t qIndex = findGraphicsQueueFamily(device);
        if (qIndex != UINT32_MAX) {
            mPhysicalDevice = device;
            mQueueFamilyIndex = qIndex;
            LOGI("PhysicalDevice: GPU selected with graphics queue family %u", qIndex);
            return true;
        }
    }

    LOGE("PhysicalDevice: No suitable GPU with graphics queue found");
    return false;
}

// グラフィックスコマンドを処理できるキューファミリーを探す
uint32_t PhysicalDevice::findGraphicsQueueFamily(vk::PhysicalDevice device) {
    std::vector<vk::QueueFamilyProperties> props = device.getQueueFamilyProperties();
    for (uint32_t i = 0; i < props.size(); ++i) {
        if (props[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            return i;
        }
    }
    return UINT32_MAX;
}
