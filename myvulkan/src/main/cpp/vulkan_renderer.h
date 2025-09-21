#pragma once
#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>
#include <thread>
#include <atomic>
#include "vulkan_context.h"
#include "swap_chain.h"
#include "surface.h"

class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    bool init(VulkanContext* vkContext, ANativeWindow* window);
    void start();
    void stop();
    void destroy();

private:
    bool createRenderPass();
    bool createFramebuffers();
    bool createCommandPoolAndBuffers();
    bool recordCommandBuffers();
    bool createSemaphores();

    void renderLoop();

private:
    VulkanContext* mVkContext = nullptr;
    Surface* mSurface = nullptr;
    SwapChain* mSwapChain = nullptr;
    VkRenderPass gRenderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> gFramebuffers;
    VkCommandPool gCommandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> gCmdBuffers;
    VkSemaphore gImageAvailable = VK_NULL_HANDLE;
    VkSemaphore gRenderFinished = VK_NULL_HANDLE;

    // スレッド制御
    std::thread gRenderThread;
    std::atomic<bool> gRun{false};
};
