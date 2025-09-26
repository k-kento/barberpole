#include "shader_module.h"
#include <stdexcept>

ShaderModule::ShaderModule(VkDevice device, const std::vector<char> &code) : mDevice(device), mModule(VK_NULL_HANDLE) {

    if (code.size() % 4 != 0) {
        throw std::runtime_error("SPIR-V code size must be a multiple of 4");
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    if (vkCreateShaderModule(mDevice, &createInfo, nullptr, &mModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }
}

ShaderModule::~ShaderModule() {
    destroy();
}

// ムーブコンストラクタ：所有権を奪う
ShaderModule::ShaderModule(ShaderModule &&other) noexcept: mDevice(other.mDevice), mModule(other.mModule) {
    other.mModule = VK_NULL_HANDLE;
}

// ムーブ代入演算子：既存リソースを解放して所有権を移す
ShaderModule &ShaderModule::operator=(ShaderModule &&other) noexcept {
    if (this != &other) {
        destroy();
        mDevice = other.mDevice;
        mModule = other.mModule;
        other.mModule = VK_NULL_HANDLE;
    }
    return *this;
}

void ShaderModule::destroy() {
    if (mModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(mDevice, mModule, nullptr);
        mModule = VK_NULL_HANDLE;
    }
}
