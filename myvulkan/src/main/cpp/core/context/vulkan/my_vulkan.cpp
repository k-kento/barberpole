//
// Created by 黒須権人 on 2025/09/26.
//

#include "my_vulkan.h"
#include <vulkan/vulkan_android.h>
#include "log.h"
#include <vector>

MyVulkan::MyVulkan() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "BarberPole";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "NoEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    // 要求する拡張
    const std::vector<const char *> requiredExtensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
    };

    // 利用可能な拡張を列挙
    uint32_t extCount = 0;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    if (res != VK_SUCCESS || extCount == 0) {
        throw std::runtime_error("Failed to enumerate instance extensions");
    }
    std::vector<VkExtensionProperties> availableExts(extCount);
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, availableExts.data());
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to query instance extensions");
    }

    // 必要な拡張がサポートされているか確認
    for (auto req: requiredExtensions) {
        bool found = false;
        for (auto &avail: availableExts) {
            if (strcmp(req, avail.extensionName) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(std::string("Required instance extension not supported: ") + req);
        }
    }

    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = requiredExtensions.size();
    instInfo.ppEnabledExtensionNames = requiredExtensions.data();

    VkResult createResult = vkCreateInstance(&instInfo, nullptr, &mInstance);
    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

MyVulkan::~MyVulkan() {
    if (mInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(mInstance, nullptr);
        mInstance = VK_NULL_HANDLE;
    }
}
