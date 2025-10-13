#pragma once

#include "device_buffer.h"

template<typename T>
class UboBuffer {
public:
    explicit UboBuffer(vk::Device &device, vk::PhysicalDevice physicalDevice)
            : mDevice(device) {
        mBuffer = std::make_unique<DeviceBuffer>(
                device,
                physicalDevice,
                sizeof(T),
                vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
        );
    }

    void update(const T &data) {
        mBuffer->copyFrom(&data, sizeof(T));
    }

    vk::Buffer getBuffer() const { return mBuffer->getBuffer(); }

    vk::DeviceSize getSize() const { return sizeof(T); }

private:
    vk::Device &mDevice;
    std::unique_ptr<DeviceBuffer> mBuffer;
};
