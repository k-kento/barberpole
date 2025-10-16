#pragma once

#include "render_pass.h"
#include <vector>
#include "kaleidoscope_instance_buffer.hpp"
#include "kaleidoscope_mesh_manager.hpp"
#include "kaleidoscope_ubo.hpp"
#include "render_strategy.hpp"
#include "../../common/texture/texture.hpp"

class KaleidoscopeRenderer : public RenderStrategy {
public:
    // TODO window Android 依存
    KaleidoscopeRenderer(VulkanContext &vkContext,
             RenderPass &renderPass,
             uint32_t windowWidth,
             uint32_t windowHeight);

    void recordDrawCommands(vk::CommandBuffer cmdBuffer) override;

    void renderFrame() override;

private:
    VulkanContext &mVkContext;

    std::unique_ptr<KaleidoscopeMeshManager> mMeshManager;
    std::unique_ptr<KaleidoscopeInstanceBuffer> mInstanceData;
    std::unique_ptr<KaleidoscopeUbo> mUbo;
    std::unique_ptr<Texture> mTexture;

    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
    RenderPass &mRenderPass;

    glm::mat4 mProjectionMatrix{};
};
