#pragma once

#include "command_buffer_helper.hpp"
#include "glm/glm.hpp"
#include "kaleidoscope_descriptor.hpp"
#include "uniform_buffer.hpp"
#include "vertex_buffer.hpp"

struct InstanceData {
    glm::mat4 model;
};

class KaleidoscopeFrameContext {
   public:
    struct UboData {
        glm::mat4 projection;
        glm::mat4 uvMatrix;
    };

    KaleidoscopeFrameContext(VulkanContext& vkContext, uint32_t instanceCount) {
        mUniformBuffer = std::make_unique<UniformBuffer<UboData>>(vkContext);
        mInstanceBuffer = std::make_unique<VertexBuffer<InstanceData>>(vkContext, instanceCount);
        mCmdBuffer = CommandBufferHelper::createCommandBuffer(vkContext);
    }

    ~KaleidoscopeFrameContext() = default;

    [[nodiscard]] UniformBuffer<UboData>& getUniformBuffer() const { return *mUniformBuffer; }

    [[nodiscard]] VertexBuffer<InstanceData> & getInstanceBuffer() const { return *mInstanceBuffer; }

    [[nodiscard]] bool hasDescriptorSet() const noexcept { return static_cast<bool>(mDescriptorSet); }

    [[nodiscard]] vk::DescriptorSet getDescriptorSet() const { return mDescriptorSet.get(); }

    void updateDescriptorSet(vk::UniqueDescriptorSet descriptorSet) { mDescriptorSet = std::move(descriptorSet); }

    vk::CommandBuffer getCmdBuffer() { return mCmdBuffer.get(); }

    bool isTextureUpdated = false;

   private:
    std::unique_ptr<UniformBuffer<UboData>> mUniformBuffer;
    std::unique_ptr<VertexBuffer<InstanceData>> mInstanceBuffer;
    vk::UniqueDescriptorSet mDescriptorSet;
    vk::UniqueCommandBuffer mCmdBuffer;
};
