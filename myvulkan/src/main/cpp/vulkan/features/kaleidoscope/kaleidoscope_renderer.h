#pragma once

#include "render_pass.h"
#include <vector>
#include "kaleidoscope_instance_buffer.hpp"
#include "kaleidoscope_mesh_manager.hpp"
#include "kaleidoscope_ubo.hpp"
#include "renderer_interface.hpp"
#include "../../core/texture/texture.hpp"
#include "render_message.hpp"
#include "rotation_state.hpp"
#include "kaleidoscope_config.hpp"

class KaleidoscopeRenderer : public RendererInterface {
public:

    // TODO window Android 依存
    KaleidoscopeRenderer(VulkanContext &vkContext,
                         RenderPass &renderPass,
                         uint32_t windowWidth,
                         uint32_t windowHeight,
                         const std::string &texturePath);

    void recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t frameIndex) override;

    void renderFrame(float deltaTimeMs, uint32_t currentFrame) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

    uint32_t getMaxFramesInFlight() override {
        return MAX_FRAMES_IN_FLIGHT;
    };

    void updateTexture(const std::string &path);

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
    RotationState mRotationState = RotationState::None;
};
