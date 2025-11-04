#pragma once

#include "render_pass.h"
#include <vector>
#include "kaleidoscope_instance_buffer.hpp"
#include "kaleidoscope_mesh_manager.hpp"
#include "kaleidoscope_ubo.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "render_message.hpp"
#include "rotation_state.hpp"

class KaleidoscopeRenderer : public Renderer {
public:

    // TODO window Android 依存
    KaleidoscopeRenderer(VulkanContext &vkContext,
                         uint32_t windowWidth,
                         uint32_t windowHeight,
                         uint32_t deviceRotationDegree,
                         const std::string &texturePath,
                         std::unique_ptr<SurfaceContext> surfaceContext);

    void renderFrame(float deltaTimeMs) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

    void updateTexture(const std::string &path);

private:
    // rad/ms
    static constexpr float rotationSpeed = glm::radians(0.1f);

    VulkanContext &mVkContext;

    std::unique_ptr<SurfaceContext> mSurfaceContext;
    std::unique_ptr<KaleidoscopeMeshManager> mMeshManager;
    std::unique_ptr<KaleidoscopeInstanceBuffer> mInstanceData;
    std::unique_ptr<KaleidoscopeUbo> mUbo;
    std::unique_ptr<Texture> mTexture;

    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;

    glm::mat4 mProjectionMatrix{};
    glm::mat4 mUvMatrix = glm::mat4(1.0f);
    float mUvAngle = 0.0f;
    RotationState mRotationState = RotationState::None;
};
