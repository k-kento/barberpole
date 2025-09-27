#pragma once
#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>
#include <thread>
#include <atomic>
#include "vulkan_context.h"
#include "swap_chain.h"
#include "surface.h"
#include "render_pass.h"
#include "frame_buffer.h"

class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    virtual bool init(VulkanContext* vkContext, ANativeWindow* window);
    bool postInit();
    void start();
    void stop();
    void destroy();
protected:
    virtual void recordDrawCommands(VkCommandBuffer cmdBuffer, uint32_t imageIndex) = 0;
    
private:
    bool createCommandPoolAndBuffers();
    bool recordCommandBuffers();
    bool createSemaphores();

    void renderLoop();
protected:
    VulkanContext* mVkContext = nullptr;
    std::unique_ptr<RenderPass> mRenderPass;
private:
    std::unique_ptr<Surface> mSurface;
    std::unique_ptr<SwapChain>  mSwapChain;

    std::unique_ptr<FrameBuffer> mFrameBuffer;

    VkCommandPool gCommandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> gCmdBuffers;
    VkSemaphore gImageAvailable = VK_NULL_HANDLE;
    VkSemaphore gRenderFinished = VK_NULL_HANDLE;

    // スレッド制御
    std::thread gRenderThread;
    std::atomic<bool> gRun{false};
};
