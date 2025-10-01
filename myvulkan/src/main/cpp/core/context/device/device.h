#pragma once

#include <vulkan/vulkan.hpp>
#include "physical_device.h"

class Device {
public:
    explicit Device(const PhysicalDevice *physicalDevice);

    ~Device() = default;

    [[nodiscard]] vk::Device getDevice() const { return mDevice.get(); }

    [[nodiscard]] vk::Queue getGraphicsQueue() const { return mGraphicsQueue; }

private:
    vk::UniqueDevice mDevice;
    vk::Queue mGraphicsQueue;
};
