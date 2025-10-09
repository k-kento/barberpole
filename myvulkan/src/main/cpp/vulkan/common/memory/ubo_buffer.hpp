#pragma once

#include "vulkan/vulkan.hpp"
#include <cstring>

template<typename T>
class UboBuffer {
public:
    explicit UboBuffer(vk::DeviceMemory memory)
            : mMemory(memory) {}

    void update(vk::Device device, const T &data) {
        void *mapped = device.mapMemory(mMemory, 0, sizeof(T));
        std::memcpy(mapped, &data, sizeof(T));
        device.unmapMemory(mMemory);
    }

private:
    vk::DeviceMemory mMemory;
};
