#pragma once

#include <android/asset_manager.h>

#include <vector>
#include <vulkan/vulkan.hpp>

#include "vulkan_context.h"
#include "vulkan_utils.h"

class ShaderHelper {
   public:
    ShaderHelper() = default;

    ~ShaderHelper() = default;

   public:
    static vk::UniqueShaderModule createShaderModule(VulkanContext& vkContext, const std::string& filePath) {
        auto* assetManager = vkContext.getAssetManager();
        auto code = VulkanUtils::readBinaryFile(assetManager, filePath);

        if (code.size() % 4 != 0) {
            throw std::runtime_error("SPIR-V code size must be a multiple of 4");
        }

        vk::ShaderModuleCreateInfo createInfo{};
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        return vkContext.getDevice().createShaderModuleUnique(createInfo);
    }

    inline static std::vector<vk::PipelineShaderStageCreateInfo> makeGraphicsStages(vk::ShaderModule vert,
                                                                                    vk::ShaderModule frag,
                                                                                    const char* vertEntry = "main",
                                                                                    const char* fragEntry = "main") {
        return {makeStage(vert, vk::ShaderStageFlagBits::eVertex, vertEntry),
                makeStage(frag, vk::ShaderStageFlagBits::eFragment, fragEntry)};
    }

    inline static std::vector<vk::PipelineShaderStageCreateInfo> makeComputeStage(vk::ShaderModule comp,
                                                                                  const char* entry = "main") {
        return {makeStage(comp, vk::ShaderStageFlagBits::eCompute, entry)};
    }

   private:
    // 共通のヘルパー：ShaderModule から StageCreateInfo を作る
    inline static vk::PipelineShaderStageCreateInfo makeStage(vk::ShaderModule module,
                                                              vk::ShaderStageFlagBits stage,
                                                              const char* entry = "main") {
        vk::PipelineShaderStageCreateInfo info{};
        info.stage = stage;
        info.module = module;
        info.pName = entry;
        return info;
    }
};
