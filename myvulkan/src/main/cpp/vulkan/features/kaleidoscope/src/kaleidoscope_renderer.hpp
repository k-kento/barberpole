#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "renderer.hpp"
#include "texture.hpp"
#include "render_message.hpp"
#include "rotation_state.hpp"
#include "surface_context.hpp"
#include "mesh_buffer.hpp"
#include "kaleidoscope_descriptor.hpp"
#include "kaleidoscope_frame_context.hpp"

struct Vertex;

class KaleidoscopeRenderer : public Renderer {
public:

    KaleidoscopeRenderer(VulkanContext &vkContext,
                         uint32_t windowWidth,
                         uint32_t windowHeight,
                         uint32_t deviceRotationDegree,
                         const std::string &texturePath,
                         std::unique_ptr<SurfaceContext> surfaceContext);

    ~KaleidoscopeRenderer() = default;

    void renderFrame(float deltaTimeMs) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

    void updateTexture(const std::string &path);

private:
    // rad/ms
    static constexpr float rotationSpeed = glm::radians(0.1f);

    VulkanContext &mVkContext;

    std::unique_ptr<SurfaceContext> mSurfaceContext;
    std::unique_ptr<MeshBuffer<Vertex>> mMesh;

    std::unique_ptr<Texture> mTexture;
    std::array<bool, SurfaceContext::MAX_FRAMES_IN_FLIGHT> mIsTextureUpdated;

    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
    std::unique_ptr<KaleidoscopeDescriptor> mDescriptor;

    glm::mat4 mProjectionMatrix{};
    glm::mat4 mUvMatrix = glm::mat4(1.0f);
    float mUvAngle = 0.0f;
    RotationState mRotationState = RotationState::None;

    void recordCommandBuffer(KaleidoscopeFrameContext* frameContext);
};
