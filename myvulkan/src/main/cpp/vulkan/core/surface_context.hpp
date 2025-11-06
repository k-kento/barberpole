#pragma once

#include "memory"
#include "swap_chain.hpp"
#include "render_pass.h"
#include "surface.h"
#include "frame_context.hpp"

class SurfaceContext {
public:
    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    using FrameContextFactory = std::function<std::unique_ptr<FrameContext>(VulkanContext&)>;

    SurfaceContext(VulkanContext &vkContext, std::shared_ptr<Surface> surface, std::vector<std::unique_ptr<FrameContext>> frameContexts);

    ~SurfaceContext() = default;

    Surface &getSurface() const { return *mSurface; }

    SwapChain &getSwapChain() { return *mSwapChain; }

    RenderPass &getRenderPass() { return *mRenderPass; }

    FrameContext *getFrameContext();

    FrameContext *getFrameContext(uint32_t index);

    void beginCommandBuffer(vk::CommandBuffer cmdBuffer);

    void endCommandBuffer(vk::CommandBuffer cmdBuffer);

    void setFrameContexts(std::vector<std::unique_ptr<FrameContext>> frameContexts);

    void acquireNextImage();
    void submit();
    void present();

    uint32_t getCurrentFrameIndex() {
        return mCurrentFrameIndex;
    }

private:
    VulkanContext &mVkContext;

    std::shared_ptr<Surface> mSurface;
    std::unique_ptr<SwapChain> mSwapChain;
    std::unique_ptr<RenderPass> mRenderPass;
    std::vector<vk::UniqueFramebuffer> mFrameBuffers;

    std::vector<std::unique_ptr<FrameContext>> mFrameContexts;
    uint32_t mCurrentFrameIndex = 0;

    std::vector<vk::UniqueFramebuffer> createFrameBuffers();
};
