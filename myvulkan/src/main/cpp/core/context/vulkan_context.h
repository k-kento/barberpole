#pragma once

#include <android/asset_manager.h>
#include <vulkan/vulkan.h>
#include "vulkan/vulkan.hpp"
#include "physical_device.h"
#include "device.h"
#include "memory"

class VulkanContext {

public:
    explicit VulkanContext(AAssetManager *assetManager) noexcept(false);

    [[nodiscard]] VkInstance getVkInstance() const {
        return mVkInstance.get();
    }

    [[nodiscard]] PhysicalDevice *getPhysicalDevice() const {
        return mPhysicalDevice.get();
    }

    [[nodiscard]] VkPhysicalDevice getVkPhysicalDevice() const {
        return mPhysicalDevice->getPhysicalDevice();
    }

    [[nodiscard]] VkDevice getVkDevice() {
        return mDevice->getDevice();
    }

    [[nodiscard]] Device *getDevice() {
        return mDevice.get();
    }

    [[nodiscard]] AAssetManager *getAssetManager() const {
        return mAssetManager;
    };

private:
    vk::UniqueInstance createVkInstance();

private:
    vk::UniqueInstance mVkInstance;
    std::unique_ptr<PhysicalDevice> mPhysicalDevice;
    std::unique_ptr<Device> mDevice;
    AAssetManager *mAssetManager = nullptr; // TODO Android 依存コード
};
