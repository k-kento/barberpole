#pragma once

#include <vulkan/vulkan.hpp>

class TextureUtils {

public:

    /**
     * 画像の状態を変える
     */
    static void transitionImageLayout(
            vk::CommandBuffer cmd,
            vk::Image image,
            vk::ImageLayout oldLayout,
            vk::ImageLayout newLayout
    ) {
        vk::ImageMemoryBarrier barrier{};
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        vk::PipelineStageFlags srcStage;
        vk::PipelineStageFlags dstStage;

        if (oldLayout == vk::ImageLayout::eUndefined &&
            newLayout == vk::ImageLayout::eTransferDstOptimal) {
            // Undefined → TransferDst
            barrier.srcAccessMask = {};
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

            srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
            dstStage = vk::PipelineStageFlagBits::eTransfer;
        } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
                   newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
            // TransferDst → ShaderReadOnly
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            srcStage = vk::PipelineStageFlagBits::eTransfer;
            dstStage = vk::PipelineStageFlagBits::eFragmentShader;
        } else {
            throw std::invalid_argument("Unsupported layout transition");
        }

        cmd.pipelineBarrier(srcStage,
                            dstStage,
                            {},
                            nullptr,
                            nullptr,
                            barrier);
    }

    /**
     * CPU でロードしたピクセルデータを GPU の画像にコピーする
     */
    static void copyBufferToImage(
            vk::CommandBuffer cmd,
            vk::Buffer buffer,
            vk::Image image,
            uint32_t width,
            uint32_t height
    ) {
        vk::BufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = vk::Offset3D{0, 0, 0};
        region.imageExtent = vk::Extent3D{width, height, 1};

        cmd.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);
    }
};
