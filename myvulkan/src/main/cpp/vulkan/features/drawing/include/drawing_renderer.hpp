#pragma once

#include "vulkan_context.h"
#include "render_pass.h"
#include <vector>
#include "renderer_interface.hpp"
#include "render_message.hpp"
#include "glm/glm.hpp"
#include "view_bounds.hpp"

class Ubo;
class InstanceBuffer;
class Mesh;

class DrawingRenderer : public RendererInterface {
public:

    DrawingRenderer(VulkanContext &vkContext, RenderPass &renderPass, ViewBounds &viewBounds, glm::mat4 projectionMatrix);

    ~DrawingRenderer();

    void recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t frameIndex) override;

    void renderFrame(float deltaTimeMs, uint32_t currentFrame) override;

    void handleMessage(std::unique_ptr<RenderMessage> message) override;

private:

    VulkanContext &mVkContext;

    std::unique_ptr<InstanceBuffer> mInstanceBuffer;
    std::unique_ptr<Ubo> mUbo;
    std::unique_ptr<Mesh> mMesh;
    vk::UniquePipeline mPipeline;
    vk::UniquePipelineLayout mPipelineLayout;
    RenderPass &mRenderPass;
    std::deque<glm::mat4> mTouchPoints;
    ViewBounds& mViewBounds;
    glm::mat4 mProjectionMatrix;
};
