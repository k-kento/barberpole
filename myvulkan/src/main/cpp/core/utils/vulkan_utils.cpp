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
