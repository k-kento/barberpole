#include <vulkan/vulkan.h>
#include <vector>
#include <android/asset_manager.h>
#include <vulkan/vulkan.hpp>
#include "swap_chain.h"
#include "view_bounds.hpp"

class VulkanUtils {
public:
    VulkanUtils() = default;

    ~VulkanUtils() = default;

    static std::vector<char> readTextFile(AAssetManager *assetManager, const std::string &filename);

    static std::vector<unsigned char> readBinaryFile(AAssetManager* manager, const std::string& filename);

    static vk::UniqueShaderModule createShaderModule(vk::Device device, const std::vector<char> &code);

    static std::vector<vk::UniqueFramebuffer> createFrameBuffers(vk::Device device, SwapChain *swapChain, vk::RenderPass renderPass);

    static glm::mat4 generateProjectionMatrix(uint32_t deviceRotationDegree, ViewBounds& viewBounds);
};
