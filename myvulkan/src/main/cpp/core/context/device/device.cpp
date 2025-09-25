
#include "device.h"
#include "log.h"

// 物理デバイスから論理デバイスを作り、グラフィックスキューを取得する
bool Device::init(PhysicalDevice *physicalDevice) {
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = physicalDevice->getQueueFamilyIndex();
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    const char *deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions;

    auto result = vkCreateDevice(physicalDevice->getPhysicalDevice(), &deviceInfo, nullptr, &mDevice);
    if (result != VK_SUCCESS) {
        LOGE("Failed to create logical device %d", result);
        return false;
    }

    vkGetDeviceQueue(mDevice, physicalDevice->getQueueFamilyIndex(), 0, &mGraphicsQueue);
    return true;
}

void Device::destroy() {
    if (mDevice != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(mDevice);
        vkDestroyDevice(mDevice, nullptr);
        mDevice = VK_NULL_HANDLE;
        mGraphicsQueue = VK_NULL_HANDLE;
    }
}
