#pragma once

#include <vulkan/vulkan.hpp>
#include "physical_device_helper.hpp"

struct DeviceBundle {
    vk::UniqueDevice device;
    vk::Queue graphicsQueue;
};

class DeviceHelper {
public:

    DeviceHelper() = delete;

    static DeviceBundle createDevice(PhysicalDeviceBundle bundle);
};
