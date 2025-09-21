#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>
#include "vulkan_context.h"

class Surface {
public:
    Surface() = default;

    ~Surface() = default;

    bool create(VulkanContext* vkContext, ANativeWindow *window);

    void destroy(VulkanContext *vkContext);

    VkSurfaceKHR getSurface();

private:
    VkSurfaceKHR mSurface = VK_NULL_HANDLE;
    ANativeWindow* mWindow = nullptr;
};
