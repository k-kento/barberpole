#pragma once

#include <vector>

#include "../input_vertex.hpp"
#include "../pipeline/base_pipeline.hpp"
#include "../renderer_constants.hpp"
#include "../ubo_data.hpp"
#include "vertex_buffer.hpp"
#include "glm/glm.hpp"
#include "log.h"
#include "uniform_buffer.hpp"
#include "vertex_buffer.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"

/**
 * Brush クラス
 *
 * 役割:
 * - 生成（CPU）: 座標データからポリゴン（三角ストリップ）を計算
 * - 転送（GPU）: MAX_FRAMES_IN_FLIGHT 分の vertexBuffer を保持し、差分更新
 * - 描画（Command）: record メソッドでバッファをバインドし描画命令を発行
 */
class Brush {
   public:
    struct FrameContext {
        std::unique_ptr<VertexBuffer<InputVertex>> vertexBuffer;
        std::unique_ptr<UniformBuffer<UboData>> uboBuffer;
        vk::UniqueDescriptorSet descriptorSet;
        uint32_t writtenVertexCount = 0;
    };

    Brush(VulkanContext& context, BasePipeline& pipeline) : mDevice(context.getDevice()), mPipeline(pipeline) {
        createDescriptorPool();
        initFrameContexts(context);
    }

    virtual ~Brush() = default;

    void setProjection(const glm::mat4& projection) {
        mProjection = projection;
        for (auto& frame : mFrames) {
            UboData ubo{};
            ubo.projection = projection;
            ubo.time = 0.0f;
            ubo.glowIntensity = 0.0f;
            ubo.glowRadius = 0.0f;
            ubo.pulsePeriod = 1.0f;
            frame.uboBuffer->update(ubo);
        }
    }

    /**
     * UBO を更新（time を含む）
     * GlowBrush でオーバーライドしてグローパラメータも更新
     */
    virtual void updateUboWithTime(uint32_t frameIndex, float time) {
        UboData ubo{};
        ubo.projection = mProjection;
        ubo.time = time;
        ubo.glowIntensity = 0.0f;
        ubo.glowRadius = 0.0f;
        ubo.pulsePeriod = 1.0f;
        mFrames[frameIndex].uboBuffer->update(ubo);
    }

    /**
     * 座標データから頂点データを生成（CPU処理）
     * @param points 入力座標列
     * @param outVertices 出力頂点バッファ
     */
    virtual void generateVertices(const std::vector<glm::vec2>& points, std::vector<InputVertex>& outVertices) = 0;

    /**
     * GPU バッファへの差分更新
     * @param frameIndex 現在のフレームインデックス
     * @param allVertices 全頂点データ
     */
    void updateGPUBuffer(uint32_t frameIndex, const std::vector<InputVertex>& allVertices) {
        auto& frame = mFrames[frameIndex];
        auto vertexCount = static_cast<uint32_t>(allVertices.size());

        if (vertexCount <= frame.writtenVertexCount) return;
        if (vertexCount > MAX_VERTEX_COUNT) {
            vertexCount = MAX_VERTEX_COUNT;
        }

        uint32_t newCount = vertexCount - frame.writtenVertexCount;
        const InputVertex* srcPtr = allVertices.data() + frame.writtenVertexCount;

        frame.vertexBuffer->update(srcPtr, newCount, frame.writtenVertexCount);
        frame.writtenVertexCount = vertexCount;
    }

    /**
     * 描画コマンドを記録
     * @param cmd コマンドバッファ
     * @param frameIndex 現在のフレームインデックス
     */
    void record(vk::CommandBuffer cmd, uint32_t frameIndex) {
        auto& frame = mFrames[frameIndex];
        LOGD("Brush::record frameIndex=%d writtenVertexCount=%d", frameIndex, frame.writtenVertexCount);
        if (frame.writtenVertexCount == 0) return;

        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.getPipeline());
        cmd.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics, mPipeline.getLayout(), 0, {frame.descriptorSet.get()}, {});

        vk::DeviceSize offsets[] = {0};
        cmd.bindVertexBuffers(0, frame.vertexBuffer->getRawBuffer(), offsets);
        cmd.draw(frame.writtenVertexCount, 1, 0, 0);
    }

    /**
     * フレームコンテキストをリセット（新しいストローク開始時など）
     */
    void resetFrameContexts() {
        for (auto& frame : mFrames) {
            frame.writtenVertexCount = 0;
        }
    }

   protected:
    vk::Device mDevice;
    BasePipeline& mPipeline;
    vk::UniqueDescriptorPool mDescriptorPool;
    std::vector<FrameContext> mFrames;
    glm::mat4 mProjection{1.0f};

    static constexpr size_t MAX_VERTEX_COUNT = 8192;

    void createDescriptorPool() {
        vk::DescriptorPoolSize poolSize{};
        poolSize.type = vk::DescriptorType::eUniformBuffer;
        poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }

    void initFrameContexts(VulkanContext& context) {
        mFrames.reserve(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            FrameContext frameContext{};

            // TODO Max Count
            frameContext.vertexBuffer = std::make_unique<VertexBuffer<InputVertex>>(context, 1000);
            frameContext.uboBuffer = std::make_unique<UniformBuffer<UboData>>(context);
            frameContext.descriptorSet =
                allocateDescriptorSet(frameContext.uboBuffer->getBuffer(), frameContext.uboBuffer->getSize());

            mFrames.emplace_back(std::move(frameContext));
        }
    }

    vk::UniqueDescriptorSet allocateDescriptorSet(vk::Buffer uboBuffer, vk::DeviceSize uboSize) {
        vk::DescriptorSetLayout layout = mPipeline.getDescriptorSetLayout();

        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = mDescriptorPool.get();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        auto sets = mDevice.allocateDescriptorSetsUnique(allocInfo);
        auto descriptorSet = std::move(sets.front());

        vk::DescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = uboSize;

        vk::WriteDescriptorSet descriptorWrite{};
        descriptorWrite.dstSet = descriptorSet.get();
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        mDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);

        return descriptorSet;
    }
};
