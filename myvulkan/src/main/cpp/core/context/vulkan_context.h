#pragma once

#include <android/asset_manager.h>
#include <vulkan/vulkan.h>
#include "my_vulkan.h"
#include "physical_device.h"
#include "device.h"
#include "memory"

class VulkanContext {

public:
    explicit VulkanContext(AAssetManager *assetManager);

    bool init();

    [[nodiscard]] MyVulkan *getVulkan() const {
        return mVulkan.get();
    }

    [[nodiscard]] PhysicalDevice *getPhysicalDevice() const {
        return mPhysicalDevice.get();
    }

    [[nodiscard]] Device *getDevice() {
        return mDevice.get();
    }

    [[nodiscard]] AAssetManager *getAssetManager() const {
        return mAssetManager;
    };

private:
    std::unique_ptr<MyVulkan> mVulkan;
    std::unique_ptr<PhysicalDevice> mPhysicalDevice;
    std::unique_ptr<Device> mDevice;
    AAssetManager *mAssetManager = nullptr; // TODO Android 依存コード

};
