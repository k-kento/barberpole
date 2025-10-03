#pragma once
#include <vulkan/vulkan.hpp>
#include <android/native_window.h>
#include <vector>
#include <thread>
#include <atomic>
#include "vulkan_context.h"
#include "swap_chain.h"
#include "surface.h"
#include "render_pass.h"

class VulkanRenderer {
public:
    VulkanRenderer(VulkanContext* vkContext, ANativeWindow* window);
    ~VulkanRenderer();

    void start();
    void stop();
protected:
    virtual void recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t imageIndex) = 0;
    virtual void renderFrame() = 0;
    bool recordCommandBuffers();
private:
    bool createCommandPool(vk::Device device, uint32_t queueFamilyIndex);
    bool createCommandBuffers(vk::Device device);
    bool createSemaphores();

    void renderLoop();
protected:
    VulkanContext* mVkContext = nullptr;
    std::unique_ptr<RenderPass> mRenderPass;
private:
    std::unique_ptr<Surface> mSurface;
    std::unique_ptr<SwapChain>  mSwapChain;
    std::vector<vk::UniqueFramebuffer> mFrameBuffer;

    vk::UniqueCommandPool mCommandPool;
    std::vector<vk::UniqueCommandBuffer> mCmdBuffers;
    vk::UniqueSemaphore mImageAvailable;
    vk::UniqueSemaphore mRenderFinished;

    // スレッド制御
    std::thread gRenderThread;
    std::atomic<bool> gRun{false};
};
