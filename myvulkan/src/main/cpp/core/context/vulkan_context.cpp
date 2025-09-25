#include <vulkan/vulkan.h>

#include "vulkan_context.h"
#include "log.h"

#include <iostream>
#include <memory>

VulkanContext::VulkanContext(AAssetManager *assetManager) {
    mAssetManager = assetManager;
}

VulkanContext::~VulkanContext() {
    destroy();
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

void VulkanContext::destroy() {
    // 破棄の順番に注意する
    if (mDevice != nullptr) mDevice->destroy();
    mDevice.reset();

    mPhysicalDevice.reset();

    if (mVulkan != nullptr) mVulkan->destroy();
    mVulkan.reset();

    LOGI("VulkanContext destroyed");
}
