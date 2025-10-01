#include <vulkan/vulkan.hpp>
#include <vector>
#include "physical_device.h"
#include "log.h"
#include <stdexcept>

PhysicalDevice::PhysicalDevice(const vk::Instance& instance) {
    if (!pickPhysicalDevice(instance)) {
        throw std::runtime_error("Failed to pick physical device");
    }
}

// Vulkan インスタンスに接続されている GPU の中から選択
bool PhysicalDevice::pickPhysicalDevice(const vk::Instance& instance) {
    std::vector<vk::PhysicalDevice> gpus = instance.enumeratePhysicalDevices();
    if (gpus.empty()) {
        LOGE("PhysicalDevice: No GPU found");
        return false;
    }

    for (auto gpu : gpus) {
        uint32_t qIndex = findGraphicsQueueFamily(gpu);
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
