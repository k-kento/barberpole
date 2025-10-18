#pragma once

#include "render_pass.h"
#include <vector>
#include "kaleidoscope_instance_buffer.hpp"
#include "kaleidoscope_mesh_manager.hpp"
#include "kaleidoscope_ubo.hpp"
#include "renderer_interface.hpp"
#include "../../common/texture/texture.hpp"

class KaleidoscopeRenderer : public RendererInterface {
public:
    enum class RotationState : int {
        None = 0,
        RotatingCW = 1,
        RotatingCCW = 2
    };

    // TODO window Android 依存
    KaleidoscopeRenderer(VulkanContext &vkContext,
             RenderPass &renderPass,
             uint32_t windowWidth,
             uint32_t windowHeight);

    void recordDrawCommands(vk::CommandBuffer cmdBuffer) override;

    void renderFrame(float deltaTimeMs) override;

    void setRotationState(RotationState state);

private:
    // rad/ms
    static constexpr float rotationSpeed = glm::radians(0.1f);

    VulkanContext &mVkContext;

    std::unique_ptr<KaleidoscopeMeshManager> mMeshManager;
    std::unique_ptr<KaleidoscopeInstanceBuffer> mInstanceData;
    std::unique_ptr<KaleidoscopeUbo> mUbo;
    std::unique_ptr<Texture> mTexture;

    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
    RenderPass &mRenderPass;

    glm::mat4 mProjectionMatrix{};
    glm::mat4 mUvMatrix = glm::mat4(1.0f);
    float mUvAngle = 0.0f;
    std::atomic<RotationState> mRotationState = RotationState::None;
};
