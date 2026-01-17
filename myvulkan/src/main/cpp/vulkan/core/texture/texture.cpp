#define STB_IMAGE_IMPLEMENTATION

#include "texture.hpp"

#include <cstring>
#include <stdexcept>

#include "command_buffer_helper.hpp"
#include "device_buffer.h"
#include "physical_device_helper.hpp"
#include "stb_image.h"
#include "texture.hpp"
#include "texture_utils.hpp"
#include "vulkan_utils.h"

Texture::Texture(VulkanContext& context, const std::string& path) : mContext(context) {
    loadImageFromFile(path);
    createImageView();
    createSampler();
}

Texture::~Texture() = default;

vk::ImageView Texture::getImageView() const {
    return *mImageView;
}

vk::Sampler Texture::getSampler() const {
    return *mSampler;
}

void Texture::loadImageFromFile(const std::string& path) {
    auto imageData = VulkanUtils::readBinaryFile(mContext.getAssetManager(), path);
    int texWidth, texHeight, texChannels;
    size_t size = imageData.size();
    stbi_uc* pixels = stbi_load_from_memory(
        imageData.data(), static_cast<int>(size), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) throw std::runtime_error("failed to load texture image: " + path);

    vk::DeviceSize imageSize = texWidth * texHeight * 4;

    createImage(texWidth, texHeight);

    auto stagingBuffer =
        DeviceBuffer(mContext,
                     imageSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    stagingBuffer.copyFrom(pixels, imageSize, 0);

    stbi_image_free(pixels);

    auto cmdBuffer = CommandBufferHelper::beginSingleTimeCommands(mContext);
    // 画像を転送できるようにする
    TextureUtils::transitionImageLayout(
        cmdBuffer.get(), *mImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
    TextureUtils::copyBufferToImage(cmdBuffer.get(), stagingBuffer.getBuffer(), *mImage, texWidth, texHeight);
    // 転送済みの画像をシェーダーようにする
    TextureUtils::transitionImageLayout(
        cmdBuffer.get(), *mImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
    CommandBufferHelper::endSingleTimeCommands(mContext, std::move(cmdBuffer));
}

void Texture::createImage(uint32_t width, uint32_t height) {
    auto device = mContext.getDevice();
    auto physicalDevice = mContext.getPhysicalDevice();

    vk::ImageCreateInfo imageInfo{};
    imageInfo.imageType = vk::ImageType::e2D;
    imageInfo.extent = vk::Extent3D{width, height, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = vk::Format::eR8G8B8A8Srgb;
    imageInfo.tiling = vk::ImageTiling::eOptimal;
    imageInfo.initialLayout = vk::ImageLayout::eUndefined;
    imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    imageInfo.samples = vk::SampleCountFlagBits::e1;
    imageInfo.sharingMode = vk::SharingMode::eExclusive;

    mImage = device.createImageUnique(imageInfo);

    vk::MemoryRequirements memReq = device.getImageMemoryRequirements(*mImage);
    auto memoryType = PhysicalDeviceHelper::findMemoryType(
        physicalDevice, memReq.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
    vk::MemoryAllocateInfo allocInfo(memReq.size, memoryType);

    mMemory = device.allocateMemoryUnique(allocInfo);
    device.bindImageMemory(*mImage, *mMemory, 0);
}

void Texture::createImageView() {
    vk::ImageViewCreateInfo viewInfo{};
    viewInfo.image = *mImage;
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.format = vk::Format::eR8G8B8A8Srgb;
    viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.layerCount = 1;

    mImageView = mContext.getDevice().createImageViewUnique(viewInfo);
}

void Texture::createSampler() {
    vk::SamplerCreateInfo samplerInfo{};
    samplerInfo.magFilter = vk::Filter::eLinear;
    samplerInfo.minFilter = vk::Filter::eLinear;
    samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
    samplerInfo.anisotropyEnable = VK_FALSE;  // 異方性フィルタリング
    samplerInfo.maxAnisotropy = 8.0f;
    samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    mSampler = mContext.getDevice().createSamplerUnique(samplerInfo);
}
