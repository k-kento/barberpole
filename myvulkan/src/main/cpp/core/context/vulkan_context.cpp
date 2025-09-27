#include <vulkan/vulkan.h>

#include "vulkan_context.h"
#include "log.h"

#include <iostream>
#include <memory>

VulkanContext::VulkanContext(AAssetManager *assetManager) {
    mAssetManager = assetManager;
}

bool VulkanContext::init() {
    mVulkan = std::make_unique<MyVulkan>();
    mPhysicalDevice = std::make_unique<PhysicalDevice>();
    mDevice = std::make_unique<Device>();

    if (!mVulkan->init()) return false;
    if (!mPhysicalDevice->init(mVulkan->getVkInstance())) return false;
    if (!mDevice->init(mPhysicalDevice.get())) return false;

    return true;
}
