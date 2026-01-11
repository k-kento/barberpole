#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "vulkan_context.h"
#include "base_pipeline.hpp"
#include "normal_pipeline.hpp"
#include "rainbow_pipeline.hpp"

class PipelineManager {
public:
    PipelineManager(VulkanContext &ctx, RenderPass &rp)
            : mContext(ctx), mRenderPass(rp) {

        mPipelines["normal"] = std::make_unique<NormalPipeline>(mContext, mRenderPass);
        mPipelines["rainbow"] = std::make_unique<RainbowPipeline>(mContext, mRenderPass);
    }

    BasePipeline &get(const std::string &brushName) {
        return *mPipelines.at(brushName);
    }

    bool has(const std::string &brushName) const {
        return mPipelines.find(brushName) != mPipelines.end();
    }

private:
    VulkanContext &mContext;
    RenderPass &mRenderPass;

    std::unordered_map<std::string, std::unique_ptr<BasePipeline>> mPipelines;
};
