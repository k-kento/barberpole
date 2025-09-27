#pragma once

#include "physical_device.h"

class Device {
public:
    explicit Device(PhysicalDevice *physicalDevice) noexcept(false);

    ~Device();

    VkDevice getDevice() {
        return mDevice;
    }

    VkQueue getGraphicsQueue() {
        return mGraphicsQueue;
    }

private:
    VkDevice mDevice = VK_NULL_HANDLE;
    VkQueue mGraphicsQueue = VK_NULL_HANDLE;
};
