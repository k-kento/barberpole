#pragma once

#include "physical_device.h"

class Device {
public:
    bool init(PhysicalDevice* physicalDevice);

    void destroy();

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
