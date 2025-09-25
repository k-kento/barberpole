#pragma once

#include "vulkan/vulkan.h"

class MyVulkan {

public:

    bool init();

    void destroy();

    [[nodiscard]] VkInstance getVkInstance() {
        return mInstance;
    }

private:

    VkInstance mInstance = VK_NULL_HANDLE;

};
