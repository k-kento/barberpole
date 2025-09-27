#include <vulkan/vulkan.h>

#include "vulkan_context.h"
#include "log.h"

#include <iostream>
#include <memory>

VulkanContext::VulkanContext(AAssetManager *assetManager) {
    mAssetManager = assetManager;
    mVulkan = std::make_unique<MyVulkan>();
    mPhysicalDevice = std::make_unique<PhysicalDevice>(mVulkan->getVkInstance());
    mDevice = std::make_unique<Device>(mPhysicalDevice.get());
}
