#pragma once

#include <vulkan/vulkan.hpp>
#include "physical_device_helper.hpp"

class DeviceHelper {
public:

    DeviceHelper() = delete;

    static vk::UniqueDevice createDevice(PhysicalDeviceBundle bundle);
};
