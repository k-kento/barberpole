#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class ShaderModule {
public:
    ShaderModule(VkDevice device, const std::vector<char> &code);

    ~ShaderModule();

    // コピーは禁止
    ShaderModule(const ShaderModule &) = delete;

    ShaderModule &operator=(const ShaderModule &) = delete;

    // ムーブは許可
    ShaderModule(ShaderModule &&other) noexcept;

    ShaderModule &operator=(ShaderModule &&other) noexcept;

    [[nodiscard]] VkShaderModule get() const { return mModule; }

private:
    void destroy();

    VkDevice mDevice;
    VkShaderModule mModule;
};
