#include "vulkan_buffer.h"
#include <stdexcept>
#include <cstring>

VulkanBuffer::VulkanBuffer(vk::Device device,
                           vk::PhysicalDevice physicalDevice,
                           vk::DeviceSize size,
                           vk::BufferUsageFlags usage,
                           vk::MemoryPropertyFlags properties)
        : mDevice(device), mSize(size) {

    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    mBuffer = mDevice.createBufferUnique(bufferInfo);

    // バッファに必要なメモリ要件を取得
    vk::MemoryRequirements requirements = mDevice.getBufferMemoryRequirements(*mBuffer);

    // GPU で使えるメモリの種類を取得
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    uint32_t memoryTypeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((requirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            memoryTypeIndex = i;
            break;
        }
    }
    if (memoryTypeIndex == UINT32_MAX) throw std::runtime_error("Failed to find suitable memory type");

    // デバイス上にメモリを確保
    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;
    mMemory = mDevice.allocateMemoryUnique(allocInfo);

    // メモリをバッファに結びつける
    mDevice.bindBufferMemory(*mBuffer, *mMemory, 0);
}

// ムーブコンストラクタ / ムーブ代入
VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
        : mDevice(other.mDevice),
          mBuffer(std::move(other.mBuffer)),
          mMemory(std::move(other.mMemory)),
          mSize(other.mSize) {
    other.mSize = 0;
}

VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept {
    if (this != &other) {
        mBuffer = std::move(other.mBuffer);
        mMemory = std::move(other.mMemory);
        mDevice = other.mDevice;
        mSize = other.mSize;
        other.mSize = 0;
    }
    return *this;
}

/**
 * CPU メモリから GPU メモリへデータをコピー
 * Note: copyFrom only works if buffer was created with HOST_VISIBLE memory
 */
void VulkanBuffer::copyFrom(const void *data, vk::DeviceSize size, vk::DeviceSize offset) {
    if (size + offset > mSize)
        throw std::runtime_error("Copy size exceeds buffer size");

    void *mapped = mDevice.mapMemory(*mMemory, offset, size);
    std::memcpy(mapped, data, static_cast<size_t>(size));
    mDevice.unmapMemory(*mMemory);
}