
#include "device.h"
#include "log.h"
#include <stdexcept>

Device::Device(const PhysicalDevice* physicalDevice) {
    float queuePriority = 1.0f;

    // キュー作成情報
    vk::DeviceQueueCreateInfo queueInfo{};
    queueInfo.queueFamilyIndex = physicalDevice->getQueueFamilyIndex();
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    // デバイス拡張
    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    // 論理デバイス作成情報
    vk::DeviceCreateInfo deviceInfo{};
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

    try {
        // UniqueDevice による RAII 管理
        mDevice = physicalDevice->getPhysicalDevice().createDeviceUnique(deviceInfo);
    } catch (vk::SystemError& err) {
        LOGE("Failed to create logical device: %s", err.what());
        throw std::runtime_error("Failed to create logical device");
    }

    // グラフィックスキュー取得
    mGraphicsQueue = mDevice->getQueue(physicalDevice->getQueueFamilyIndex(), 0);

    LOGI("Logical device created.");
}
