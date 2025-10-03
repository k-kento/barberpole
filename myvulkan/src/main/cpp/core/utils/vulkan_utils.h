#include <vulkan/vulkan.h>
#include <vector>
#include <android/asset_manager.h>
#include <vulkan/vulkan.hpp>
#include "swap_chain.h"

class VulkanUtils {
public:
    VulkanUtils() = default;

    ~VulkanUtils() = default;

    static std::vector<char> readFile(AAssetManager *assetManager, const std::string &filename);

    static vk::UniqueShaderModule createShaderModule(vk::Device device, const std::vector<char> &code);

    static std::vector<vk::UniqueFramebuffer> createFrameBuffers(vk::Device device, SwapChain *swapChain, vk::RenderPass renderPass);
};
