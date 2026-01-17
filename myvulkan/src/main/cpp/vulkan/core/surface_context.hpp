#pragma once

#include "memory"
#include "render_pass.h"
#include "surface.h"
#include "swap_chain.hpp"

class SurfaceContext {
   public:
    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    SurfaceContext(VulkanContext& vkContext, std::shared_ptr<Surface> surface);

    ~SurfaceContext() = default;

    Surface& getSurface() const { return *mSurface; }

    SwapChain& getSwapChain() { return *mSwapChain; }

    RenderPass& getRenderPass() { return *mRenderPass; }

    void beginCommandBuffer(vk::CommandBuffer cmdBuffer);
    void beginRenderPass(vk::CommandBuffer cmdBuffer);
    void endRenderPass(vk::CommandBuffer cmdBuffer);
    void endCommandBuffer(vk::CommandBuffer cmdBuffer);

    void acquireNextImage();
    void submit(vk::CommandBuffer commandBuffer);
    void present();

    uint32_t getCurrentFrameIndex() { return mCurrentFrameIndex; }

   private:
    VulkanContext& mVkContext;

    std::shared_ptr<Surface> mSurface;
    std::unique_ptr<SwapChain> mSwapChain;
    std::unique_ptr<RenderPass> mRenderPass;
    std::vector<vk::UniqueFramebuffer> mFrameBuffers;

    std::vector<vk::UniqueFence> mInFlightFences;
    uint32_t mCurrentFrameIndex = 0;

    std::vector<vk::UniqueFramebuffer> createFrameBuffers();
};
