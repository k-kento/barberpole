#include <vulkan/vulkan.h>
#include <vector>
#include <android/asset_manager.h>

class VulkanUtils {
public:
    VulkanUtils() = default;

    ~VulkanUtils() = default;

    static std::vector<char> readFile(AAssetManager *assetManager, const std::string &filename);
};
