#include "vulkan_context.h"
#include "log.h"

VulkanContext::VulkanContext(AAssetManager *assetManager) {
    mAssetManager = assetManager;
    mVkInstance = createVkInstance();
    mPhysicalDevice = std::make_unique<PhysicalDevice>(mVkInstance.get());
    mDevice = std::make_unique<Device>(mPhysicalDevice.get());
    auto device = mDevice->getDevice();
    mGraphicsCommandPool = createCommandPool(device,
                                             mPhysicalDevice->getQueueFamilyIndex(),
                                             vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    mTransientCommandPool = createCommandPool(device,
                                             mPhysicalDevice->getQueueFamilyIndex(),
                                             vk::CommandPoolCreateFlagBits::eTransient);
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
