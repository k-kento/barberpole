#include "vulkan_utils.h"

#include <android/asset_manager_jni.h>

#include "log.h"
#include "view_bounds.hpp"

std::vector<char> VulkanUtils::readTextFile(AAssetManager* assetManager, const std::string& filename) {
    std::vector<char> buffer;

    AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to open asset: %s", filename.c_str());
        return {};
    }

    off_t length = AAsset_getLength(asset);
    buffer.resize(length);

    int64_t readSize = AAsset_read(asset, buffer.data(), length);
    if (readSize != length) {
        LOGE("Failed to read asset completely: %s", filename.c_str());
        buffer.clear();
    }

    AAsset_close(asset);
    return buffer;
}

std::vector<unsigned char> VulkanUtils::readBinaryFile(AAssetManager* assetManager, const std::string& filename) {
    std::vector<unsigned char> buffer;

    // バイナリモードで開く（STREAMING の方が大きいファイルに向く）
    AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        LOGE("Failed to open binary asset: %s", filename.c_str());
        return {};
    }

    // ファイルサイズ取得
    off_t length = AAsset_getLength(asset);
    if (length <= 0) {
        LOGE("Asset has invalid size: %s", filename.c_str());
        AAsset_close(asset);
        return {};
    }

    buffer.resize(static_cast<size_t>(length));

    // ファイルをすべて読み込む
    int64_t readSize = AAsset_read(asset, buffer.data(), length);
    if (readSize != length) {
        LOGE("Failed to read binary asset completely: %s", filename.c_str());
        buffer.clear();
    }

    AAsset_close(asset);
    return buffer;
}

std::vector<vk::UniqueFramebuffer> VulkanUtils::createFrameBuffers(vk::Device device,
                                                                   SwapChain* swapChain,
                                                                   vk::RenderPass renderPass) {
    std::vector<vk::UniqueFramebuffer> frameBuffers;

    auto imageViews = swapChain->getImageViews();  // 生ハンドルを取得
    frameBuffers.reserve(imageViews.size());

    for (auto& imageView : imageViews) {
        vk::FramebufferCreateInfo fbInfo(
            {}, renderPass, 1, &imageView, swapChain->getExtent().width, swapChain->getExtent().height, 1);

        try {
            frameBuffers.push_back(device.createFramebufferUnique(fbInfo));
        } catch (const vk::SystemError& e) {
            throw std::runtime_error(std::string("Failed to create framebuffer: ") + e.what());
        }
    }

    LOGI("Frame Buffers created.");

    return frameBuffers;
}

glm::mat4 VulkanUtils::generateProjectionMatrix(uint32_t deviceRotationDegree, ViewBounds& viewBounds) {
    float deviceRotationRad = glm::radians(static_cast<float>(deviceRotationDegree));
    auto deviceRotation = glm::rotate(glm::mat4(1.0f), deviceRotationRad, glm::vec3(0.0f, 0.0f, 1.0f));
    return deviceRotation * viewBounds.toOrthoMatrix();
}
