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

    ~VulkanContext();

    bool init();

    void destroy();

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
    std::unique_ptr<MyVulkan> mVulkan = nullptr;
    std::unique_ptr<PhysicalDevice> mPhysicalDevice = nullptr;
    std::unique_ptr<Device> mDevice = nullptr;
    AAssetManager *mAssetManager = nullptr; // TODO Android 依存コード

};
