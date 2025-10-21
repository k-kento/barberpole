
#include "device_helper.hpp"
#include <stdexcept>

vk::UniqueDevice DeviceHelper::createDevice(PhysicalDeviceBundle bundle) {
    float queuePriority = 1.0f;

    auto physicalDevice = bundle.physicalDevice;
    auto queueFamilyIndex = bundle.queueFamilyIndex;

    // キュー作成情報
    vk::DeviceQueueCreateInfo queueInfo{};
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    // デバイス拡張
    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    // 論理デバイス作成情報
    vk::DeviceCreateInfo deviceInfo{};
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

    try {
        return physicalDevice.createDeviceUnique(deviceInfo);
    } catch (vk::SystemError &err) {
        throw std::runtime_error("Failed to create logical device");
    }
}
