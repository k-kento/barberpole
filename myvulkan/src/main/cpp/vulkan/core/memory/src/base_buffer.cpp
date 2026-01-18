#include "base_buffer.hpp"

#include <cstring>
#include <stdexcept>

#include "physical_device_helper.hpp"

BaseBuffer::BaseBuffer(VulkanContext& context,
                   vk::DeviceSize size,
                   vk::BufferUsageFlags usage,
                   vk::MemoryPropertyFlags properties)
    : mContext(context), mSize(size) {
    const auto& physicalDevice = context.getPhysicalDevice();
    auto device = context.getDevice();

    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    mBuffer = device.createBufferUnique(bufferInfo);

    vk::MemoryRequirements requirements = device.getBufferMemoryRequirements(*mBuffer);
    uint32_t memoryTypeIndex = PhysicalDeviceHelper::findMemoryType(physicalDevice, requirements.memoryTypeBits, properties);

    // デバイス上にメモリを確保
    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;
    mMemory = device.allocateMemoryUnique(allocInfo);

    // メモリをバッファに結びつける
    device.bindBufferMemory(*mBuffer, *mMemory, 0);
}

BaseBuffer::BaseBuffer(BaseBuffer&& other) noexcept
    : mContext(other.mContext),
      mBuffer(std::move(other.mBuffer)),
      mMemory(std::move(other.mMemory)),
      mSize(other.mSize) {
    other.mSize = 0;
}

BaseBuffer& BaseBuffer::operator=(BaseBuffer&& other) noexcept {
    if (this != &other) {
        mBuffer = std::move(other.mBuffer);
        mMemory = std::move(other.mMemory);
        mSize = other.mSize;
        other.mSize = 0;
    }
    return *this;
}

/**
 * CPU メモリから GPU メモリへデータをコピー
 * Note: copyFrom only works if buffer was created with HOST_VISIBLE memory
 */
void BaseBuffer::copyFrom(const void* data, vk::DeviceSize size, vk::DeviceSize offset) {
    if (size + offset > mSize) throw std::runtime_error("Copy size exceeds buffer size");

    auto device = mContext.getDevice();
    void* mapped = device.mapMemory(*mMemory, offset, size);
    std::memcpy(mapped, data, static_cast<size_t>(size));
    device.unmapMemory(*mMemory);
}
