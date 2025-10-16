#pragma once

#include <android/asset_manager.h>
#include "vulkan/vulkan.hpp"
#include "physical_device.h"
#include "device.h"
#include "memory"

class VulkanContext {

public:
    explicit VulkanContext(AAssetManager *assetManager) noexcept(false);

    [[nodiscard]] vk::Instance getVkInstance() const {
        return mVkInstance.get();
    }

    [[nodiscard]] PhysicalDevice getPhysicalDevice() const {
        return *mPhysicalDevice.get();
    }

    [[nodiscard]] vk::PhysicalDevice getVkPhysicalDevice() const {
        return mPhysicalDevice->getPhysicalDevice();
    }

    [[nodiscard]] vk::Device getVkDevice() {
        return mDevice->getDevice();
    }

    [[nodiscard]] Device &getDevice() {
        return *mDevice.get();
    }

    [[nodiscard]] const vk::CommandPool getGraphicsCommandPool() const noexcept {
        return mGraphicsCommandPool.get();
    };

    // 一時的にしか使用しない Command buffer 用の Command Pool
    // テクスチャ転送、レイアウト遷移、バッファコピー等で使用
    [[nodiscard]] const vk::CommandPool getTransientCommandPool() const noexcept {
        return mTransientCommandPool.get();
    };

    [[nodiscard]] AAssetManager *getAssetManager() const {
        return mAssetManager;
    };

private:
    vk::UniqueInstance createVkInstance();
    static vk::UniqueCommandPool createCommandPool(const vk::Device &device,
                                                   const uint32_t queueFamilyIndex,
                                                   const vk::CommandPoolCreateFlagBits flagBits);

private:
    vk::UniqueInstance mVkInstance;
    std::unique_ptr<PhysicalDevice> mPhysicalDevice;
    std::unique_ptr<Device> mDevice;
    vk::UniqueCommandPool mGraphicsCommandPool;
    vk::UniqueCommandPool mTransientCommandPool;

    AAssetManager *mAssetManager = nullptr; // TODO Android 依存コード

};
