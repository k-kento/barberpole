#include <vector>
#include <android/asset_manager.h>
#include <vulkan/vulkan.hpp>
#include "vulkan_context.h"
#include "vulkan_utils.h"

class ShaderHelper {
public:
    ShaderHelper() = default;

    ~ShaderHelper() = default;

public:

    static vk::UniqueShaderModule createShaderModule(VulkanContext &vkContext, const std::string &filePath) {
        auto *assetManager = vkContext.getAssetManager();
        auto code = VulkanUtils::readBinaryFile(assetManager, filePath);

        if (code.size() % 4 != 0) {
            throw std::runtime_error("SPIR-V code size must be a multiple of 4");
        }

        vk::ShaderModuleCreateInfo createInfo{};
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        return vkContext.getDevice().createShaderModuleUnique(createInfo);
    }
};
