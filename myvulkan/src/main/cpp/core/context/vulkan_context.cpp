#include "vulkan_context.h"
#include "log.h"

VulkanContext::VulkanContext(AAssetManager *assetManager) {
    mAssetManager = assetManager;
    mVkInstance = createVkInstance();
    mPhysicalDevice = std::make_unique<PhysicalDevice>(mVkInstance.get());
    mDevice = std::make_unique<Device>(mPhysicalDevice.get());
    LOGI("VulkanContext created.");
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

