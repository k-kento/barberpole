#include "vulkan_context.h"
#include "log.h"
#include "physical_device_helper.hpp"

VulkanContext::VulkanContext(AAssetManager *assetManager) {
    mAssetManager = assetManager;
    mVkInstance = createVkInstance();

    mPhysicalDeviceBundle = PhysicalDeviceHelper::pickPhysicalDevice(mVkInstance.get());
    mDevice = DeviceHelper::createDevice(mPhysicalDeviceBundle);
    mGraphicsQueue = mDevice->getQueue(mPhysicalDeviceBundle.queueFamilyIndex, 0);

    mGraphicsCommandPool = createCommandPool(mDevice.get(),
                                             mPhysicalDeviceBundle.queueFamilyIndex,
                                             vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    mTransientCommandPool = createCommandPool(mDevice.get(),
                                              mPhysicalDeviceBundle.queueFamilyIndex,
                                              vk::CommandPoolCreateFlagBits::eTransient);
    LOGI("VulkanContext created.");
}

VulkanContext::~VulkanContext() {
    LOGI("VulkanContext destroyed.");
}

vk::UniqueInstance VulkanContext::createVkInstance() {
    vk::ApplicationInfo appInfo{
            "BarberPole",
            VK_MAKE_VERSION(1, 0, 0),
            "NoEngine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_1
    };

    std::vector<const char *> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
    };

    vk::InstanceCreateInfo instInfo{};
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instInfo.ppEnabledExtensionNames = extensions.data();

    return vk::createInstanceUnique(instInfo);
}

vk::UniqueCommandPool VulkanContext::createCommandPool(const vk::Device &device,
                                                       const uint32_t queueFamilyIndex,
                                                       const vk::CommandPoolCreateFlagBits flagBits) {
    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = flagBits;

    try {
        return device.createCommandPoolUnique(poolInfo);
    } catch (const vk::SystemError &e) {
        throw std::runtime_error(std::string("Failed to create command pool: ") + e.what());
    }
}
