#pragma once
#include <vulkan/vulkan.h>
#include <cstddef>

class VulkanBuffer {
public:
    VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice,
                 VkDeviceSize size, VkBufferUsageFlags usage,
                 VkMemoryPropertyFlags properties);
    ~VulkanBuffer();

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

    VulkanBuffer(VulkanBuffer&& other) noexcept;
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    VkBuffer getBuffer() const { return mBuffer; }
    VkDeviceMemory getMemory() const { return mMemory; }
    VkDeviceSize getSize() const { return mSize; }

    void copyFrom(const void* data, VkDeviceSize size, VkDeviceSize offset = 0);

private:
    void destroy();

    VkDevice mDevice;
    VkBuffer mBuffer;
    VkDeviceMemory mMemory;
    VkDeviceSize mSize;
};