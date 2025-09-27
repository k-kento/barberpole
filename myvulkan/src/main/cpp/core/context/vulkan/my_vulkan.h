#pragma once

#include "vulkan/vulkan.h"

class MyVulkan {

public:

    MyVulkan() noexcept(false);

    ~MyVulkan();

    [[nodiscard]] VkInstance getVkInstance() noexcept {
        return mInstance;
    }

private:

    VkInstance mInstance = VK_NULL_HANDLE;

};
