#pragma once

#include "vulkan_context.h"
#include "render_pass.h"
#include <vector>
#include "renderer.hpp"
#include "render_message.hpp"
#include "glm/glm.hpp"
#include "view_bounds.hpp"
#include "surface_context.hpp"
#include "renderer.hpp"

class Ubo;
class InstanceBuffer;
class Mesh;

class DrawingRenderer : public Renderer {
public:

    DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surface);

    ~DrawingRenderer();

    void renderFrame(float deltaTimeMs) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

private:

    VulkanContext &mVkContext;
    std::unique_ptr<SurfaceContext> mSurfaceContext;
    std::unique_ptr<InstanceBuffer> mInstanceBuffer;
    std::unique_ptr<Ubo> mUbo;
    std::unique_ptr<Mesh> mMesh;
    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
    std::deque<glm::mat4> mTouchPoints;
    glm::mat4 mProjection{1.0f};
    ViewBounds mViewBounds{0.0f, 0.0f, 0.0f, 0.0f};
};
