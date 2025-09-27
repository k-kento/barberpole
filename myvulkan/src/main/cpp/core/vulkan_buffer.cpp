#include "vulkan_buffer.h"
#include <stdexcept>
#include <cstring>

VulkanBuffer::VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice,
                           VkDeviceSize size, VkBufferUsageFlags usage,
                           VkMemoryPropertyFlags properties)
        : mDevice(device), mBuffer(VK_NULL_HANDLE), mMemory(VK_NULL_HANDLE), mSize(size) {

    // バッファ作成
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer");
    }

    // メモリ要件取得
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, mBuffer, &memRequirements);

    // メモリ割り当て情報
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    bool found = false;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            allocInfo.memoryTypeIndex = i;
            found = true;
            break;
        }
    }

    if (!found) throw std::runtime_error("Failed to find suitable memory type");

    if (vkAllocateMemory(device, &allocInfo, nullptr, &mMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory");
    }

    vkBindBufferMemory(device, mBuffer, mMemory, 0);
}

VulkanBuffer::~VulkanBuffer() {
    destroy();
}

VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
        : mDevice(other.mDevice), mBuffer(other.mBuffer), mMemory(other.mMemory), mSize(other.mSize) {
    other.mBuffer = VK_NULL_HANDLE;
    other.mMemory = VK_NULL_HANDLE;
    other.mSize = 0;
}

VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept {
    if (this != &other) {
        destroy();
        mDevice = other.mDevice;
        mBuffer = other.mBuffer;
        mMemory = other.mMemory;
        mSize = other.mSize;

        other.mBuffer = VK_NULL_HANDLE;
        other.mMemory = VK_NULL_HANDLE;
        other.mSize = 0;
    }
    return *this;
}

void VulkanBuffer::copyFrom(const void *data, VkDeviceSize size, VkDeviceSize offset) {
    if (size + offset > mSize) throw std::runtime_error("Copy size exceeds buffer size");

    void *mapped;
    vkMapMemory(mDevice, mMemory, offset, size, 0, &mapped);
    std::memcpy(mapped, data, static_cast<size_t>(size));
    vkUnmapMemory(mDevice, mMemory);
}

void VulkanBuffer::destroy() {
    if (mBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(mDevice, mBuffer, nullptr);
        mBuffer = VK_NULL_HANDLE;
    }
    if (mMemory != VK_NULL_HANDLE) {
        vkFreeMemory(mDevice, mMemory, nullptr);
        mMemory = VK_NULL_HANDLE;
    }
    mSize = 0;
}