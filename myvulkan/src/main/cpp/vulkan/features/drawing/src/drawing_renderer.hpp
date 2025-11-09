#pragma once

#include "vulkan_context.h"
#include "render_pass.h"
#include <vector>
#include "renderer.hpp"
#include "render_message.hpp"
#include "glm/glm.hpp"
#include "view_bounds.hpp"
#include "surface_context.hpp"
#include "graphic_descriptor.hpp"
#include "frame_context.hpp"
#include "input_buffer.hpp"
#include "compute_pipeline.hpp"
#include "compute_buffer.hpp"
#include "graphic_pipeline.hpp"

class DrawingRenderer : public Renderer {
public:

    DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surface);

    ~DrawingRenderer();

    void renderFrame(float deltaTimeMs) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

private:


    VulkanContext &mVkContext;
    std::unique_ptr<SurfaceContext> mSurfaceContext;

    std::vector<std::unique_ptr<FrameContext>> mFrameContexts;

    std::unique_ptr<InputBuffer> mInputBuffer;
    std::deque<InputVertex> mTouchPoints;
    glm::mat4 mProjection{1.0f};
    ViewBounds mViewBounds{0.0f, 0.0f, 0.0f, 0.0f};

    std::unique_ptr<GraphicPipeline> mGraphicPipeline;
    std::unique_ptr<GraphicDescriptor> mGraphicDescriptor;

    std::unique_ptr<ComputeBuffer> mComputeBuffer;
    std::unique_ptr<ComputePipeline> mComputePipeline;
    std::unique_ptr<ComputeDescriptor> mComputeDescriptor;
};
