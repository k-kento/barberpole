#pragma once

#include "kaleidoscope_descriptor.hpp"
#include "glm/glm.hpp"
#include "ubo_buffer.hpp"
#include "kaleidoscope_instance_buffer.hpp"
#include "command_buffer_helper.hpp"

class KaleidoscopeFrameContext {
public:

    struct UboData {
        glm::mat4 projection;
        glm::mat4 uvMatrix;
    };

    KaleidoscopeFrameContext(VulkanContext &vkContext, ViewBounds &viewBounds) {
        mUniformBuffer = std::make_unique<UboBuffer<UboData>>(vkContext);
        mInstanceBuffer = std::make_unique<KaleidoscopeInstanceBuffer>(vkContext, viewBounds);
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
    }

    ~KaleidoscopeFrameContext() = default;

    [[nodiscard]] UboBuffer<UboData> &getUniformBuffer() const {
        return *mUniformBuffer;
    }

    [[nodiscard]] KaleidoscopeInstanceBuffer &getInstanceBuffer() const {
        return *mInstanceBuffer;
    }

    [[nodiscard]] bool hasDescriptorSet() const noexcept {
        return static_cast<bool>(mDescriptorSet);
    }

    [[nodiscard]] vk::DescriptorSet getDescriptorSet() const {
        return mDescriptorSet.get();
    }

    void updateDescriptorSet(vk::UniqueDescriptorSet descriptorSet) {
        mDescriptorSet = std::move(descriptorSet);
    }

    vk::CommandBuffer getCmdBuffer() {
        return mCmdBuffer.get();
    }

    bool isTextureUpdated = false;

private:

    std::unique_ptr<UboBuffer<UboData>> mUniformBuffer;
    std::unique_ptr<KaleidoscopeInstanceBuffer> mInstanceBuffer;
    vk::UniqueDescriptorSet mDescriptorSet;
    vk::UniqueCommandBuffer mCmdBuffer;

};
