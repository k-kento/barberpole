#include "vulkan_utils.h"
#include "log.h"
#include <android/asset_manager_jni.h>

std::vector<char> VulkanUtils::readFile(AAssetManager *assetManager, const std::string &filename) {
    std::vector<char> buffer;

    AAsset *asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
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

vk::UniqueShaderModule VulkanUtils::createShaderModule(vk::Device device, const std::vector<char> &code) {
    if (code.size() % 4 != 0) {
        throw std::runtime_error("SPIR-V code size must be a multiple of 4");
    }

    vk::ShaderModuleCreateInfo createInfo{};
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    return device.createShaderModuleUnique(createInfo);
}

std::vector<vk::UniqueFramebuffer>
VulkanUtils::createFrameBuffers(vk::Device device, SwapChain *swapChain, vk::RenderPass renderPass) {
    std::vector<vk::UniqueFramebuffer> frameBuffers;

    auto imageViews = swapChain->getImageViews(); // 生ハンドルを取得
    frameBuffers.reserve(imageViews.size());

    for (auto &imageView: imageViews) {
        vk::FramebufferCreateInfo fbInfo(
                {},
                renderPass,
                1,
                &imageView,
                swapChain->getExtent().width,
                swapChain->getExtent().height,
                1
        );

        try {
            frameBuffers.push_back(device.createFramebufferUnique(fbInfo));
        } catch (const vk::SystemError &e) {
            throw std::runtime_error(std::string("Failed to create framebuffer: ") + e.what());
        }
    }

    LOGI("Frame Buffers created.");

    return frameBuffers;
}