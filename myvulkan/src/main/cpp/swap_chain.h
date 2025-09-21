#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>
#include "vulkan_context.h"

class SwapChain {
public:
    SwapChain() = default;

    ~SwapChain() = default;

    bool create(VulkanContext *vkContext, VkSurfaceKHR surface);

    void destroy(VkDevice device);

    VkSwapchainKHR getSwapChain() const;

    const std::vector<VkImageView> &getImageViews();

    VkFormat getFormat();

    VkExtent2D getExtent();

private:
    bool createImageViews(VulkanContext *vkContext);

private:
    VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mImageViews;
    VkFormat mFormat;
    VkExtent2D mExtent;
};
