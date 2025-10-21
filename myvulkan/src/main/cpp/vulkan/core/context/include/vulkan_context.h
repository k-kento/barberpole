#pragma once

#include <android/asset_manager.h>
#include "vulkan/vulkan.hpp"
#include "physical_device_helper.hpp"
#include "device_helper.hpp"
#include "physical_device_helper.hpp"

class VulkanContext {

public:
    explicit VulkanContext(AAssetManager *assetManager) noexcept(false);

    ~VulkanContext();

    [[nodiscard]] vk::Instance getVkInstance() const {
        return mVkInstance.get();
    }

    [[nodiscard]] vk::PhysicalDevice getPhysicalDevice() const {
        return mPhysicalDeviceBundle.physicalDevice;
    }

    [[nodiscard]] PhysicalDeviceBundle getPhysicalDeviceBundle() const {
        return mPhysicalDeviceBundle;
    }

    [[nodiscard]] vk::Device getDevice() {
        return mDevice.get();
    }

    [[nodiscard]] vk::Queue getGraphicsQueue() const {
        return mGraphicsQueue;
    }

    [[nodiscard]] vk::CommandPool getGraphicsCommandPool() const noexcept {
        return mGraphicsCommandPool.get();
    };

    // 一時的にしか使用しない Command buffer 用の Command Pool
    // テクスチャ転送、レイアウト遷移、バッファコピー等で使用
    [[nodiscard]] vk::CommandPool getTransientCommandPool() const noexcept {
        return mTransientCommandPool.get();
    };

    [[nodiscard]] AAssetManager *getAssetManager() const {
        return mAssetManager;
    };

private:
    static vk::UniqueInstance createVkInstance();

    static vk::UniqueCommandPool createCommandPool(const vk::Device &device,
                                                   uint32_t queueFamilyIndex,
                                                   vk::CommandPoolCreateFlagBits flagBits);

private:
    vk::UniqueInstance mVkInstance;
    PhysicalDeviceBundle mPhysicalDeviceBundle;

    vk::UniqueDevice mDevice;
    vk::Queue mGraphicsQueue;

    vk::UniqueCommandPool mGraphicsCommandPool;
    vk::UniqueCommandPool mTransientCommandPool;

    AAssetManager *mAssetManager = nullptr; // TODO Android 依存コード

};
