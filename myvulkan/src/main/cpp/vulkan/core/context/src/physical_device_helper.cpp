#include "physical_device_helper.hpp"
#include "log.h"

// 接続されているデバイスの中からグラフィックスコマンドを処理できるものを探す
PhysicalDeviceBundle PhysicalDeviceHelper::pickPhysicalDevice(const vk::Instance &instance) {
    std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
    if (devices.empty()) {
        throw std::runtime_error("PhysicalDevice: No GPU found.");
    }

    for (auto device: devices) {
        std::vector<vk::QueueFamilyProperties> props = device.getQueueFamilyProperties();
        for (uint32_t i = 0; i < props.size(); ++i) {
            if (props[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                return PhysicalDeviceBundle{device, i};
            }
        }
    }
    throw std::runtime_error("PhysicalDevice: No suitable GPU with graphics queue found.");
}

// 指定したプロパティを満たすメモリタイプのインデックスを返す
uint32_t PhysicalDeviceHelper::findMemoryType(const vk::PhysicalDevice &physicalDevice,
                                              uint32_t typeFilter,
                                              vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type.");
}