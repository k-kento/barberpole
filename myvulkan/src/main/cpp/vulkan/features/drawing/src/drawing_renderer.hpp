#pragma once

#include "vulkan_context.h"
#include "render_pass.h"
#include <vector>
#include <deque>
#include "renderer.hpp"
#include "render_message.hpp"
#include "glm/glm.hpp"
#include "view_bounds.hpp"
#include "surface_context.hpp"
#include "frame_context.hpp"
#include "stroke/stroke_manager.hpp"
#include "pipeline/pipeline_manager.hpp"
#include "brush/brush_manager.hpp"

class DrawingRenderer : public Renderer {
public:

    DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surface);

    ~DrawingRenderer();

    void renderFrame(float deltaTimeMs) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

private:

    // ==============================
    // Contexts
    // ==============================
    VulkanContext &mVkContext;
    std::unique_ptr<SurfaceContext> mSurfaceContext;
    std::vector<std::unique_ptr<FrameContext>> mFrameContexts;

    // ==============================
    // Input
    // ==============================
    std::unique_ptr<StrokeManager> mStrokeManager;
    std::unique_ptr<PipelineManager> mPipelineManager;
    std::unique_ptr<BrushManager> mBrushManager;


    // ==============================
    // Render State
    // ==============================
    glm::mat4 mProjection{1.0f};
    ViewBounds mViewBounds{0.0f, 0.0f, 0.0f, 0.0f};
    float mElapsedTime = 0.0f;
    std::chrono::steady_clock::time_point mStartTime = std::chrono::steady_clock::now();

    FrameContext &getCurrentFrameContext();
    void beginFrame(FrameContext &frameContext, vk::CommandBuffer cmdBuffer);
    void updateUniforms(FrameContext &frameContext);
    void recordGraphicsPass(vk::CommandBuffer cmdBuffer, FrameContext &frameContext);
    void endFrame(vk::CommandBuffer cmdBuffer, FrameContext &frameContext);
};
