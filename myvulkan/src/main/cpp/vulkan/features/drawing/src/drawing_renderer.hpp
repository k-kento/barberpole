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
#include "ubo_buffer.hpp"
#include "descriptor.hpp"

class InstanceBuffer;
class Mesh;


class DrawingRenderer : public Renderer {
public:

    DrawingRenderer(VulkanContext &vkContext, std::unique_ptr<SurfaceContext> surface);

    ~DrawingRenderer();

    void renderFrame(float deltaTimeMs) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

private:

    struct UboData {
        glm::mat4 projection;
    };

    struct FrameResource {
        std::unique_ptr<InstanceBuffer> instanceBuffer;
        std::unique_ptr<UboBuffer<UboData>> uboBuffer;
        vk::UniqueDescriptorSet descriptorSet;
    };

    VulkanContext &mVkContext;
    std::unique_ptr<SurfaceContext> mSurfaceContext;

    std::vector<FrameResource> frameResources;

    std::unique_ptr<Mesh> mMesh;
    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
    std::unique_ptr<Descriptor> mDescriptor;
    std::deque<glm::mat4> mTouchPoints;
    glm::mat4 mProjection{1.0f};
    ViewBounds mViewBounds{0.0f, 0.0f, 0.0f, 0.0f};
};
