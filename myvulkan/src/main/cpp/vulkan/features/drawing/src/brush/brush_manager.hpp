#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "brush.hpp"
#include "normal/normal_brush.hpp"
#include "rainbow/rainbow_brush.hpp"
#include "glow/glow_brush.hpp"
#include "star/star_brush.hpp"
#include "circle/circle_brush.hpp"
#include "../pipeline/pipeline_manager.hpp"
#include "log.h"

/**
 * BrushManager - ブラシの管理クラス
 *
 * 複数のブラシを管理し、現在のブラシを切り替える
 */
class BrushManager {
public:
    enum class Type {
        Normal,
        Rainbow,
        Glow,
        Star,
        Circle
    };

    BrushManager(VulkanContext& vulkanContext, PipelineManager& pipelineManager) {
        LOGD("BrushManager: registering brushes...");
        registerBrush(Type::Normal, std::make_unique<NormalBrush>(vulkanContext, pipelineManager.get("normal")));
        registerBrush(Type::Rainbow, std::make_unique<RainbowBrush>(vulkanContext, pipelineManager.get("rainbow")));
        registerBrush(Type::Glow, std::make_unique<GlowBrush>(vulkanContext, pipelineManager.get("glow")));
        registerBrush(Type::Star, std::make_unique<StarBrush>(vulkanContext, pipelineManager.get("star")));
        registerBrush(Type::Circle, std::make_unique<CircleBrush>(vulkanContext, pipelineManager.get("circle")));
        LOGD("BrushManager: setting default to Star");
        set(Type::Star);  // デフォルトを Star に変更
    }

    void set(Type type) {
        if (mBrushes.find(type) == mBrushes.end()) {
            throw std::runtime_error("Brush type not registered.");
        }
        mCurrent = type;
        LOGD("BrushManager: current brush set to %d", static_cast<int>(type));
    }

    Brush& current() {
        return *mBrushes.at(mCurrent);
    }

    Brush* currentPtr() {
        return mBrushes.at(mCurrent).get();
    }

    const Brush& current() const {
        return *mBrushes.at(mCurrent);
    }

    Type getCurrentType() const {
        return mCurrent;
    }

private:
    struct TypeHash {
        std::size_t operator()(Type t) const noexcept {
            return static_cast<std::size_t>(t);
        }
    };

    void registerBrush(Type type, std::unique_ptr<Brush> brush) {
        mBrushes.emplace(type, std::move(brush));
    }

    std::unordered_map<Type, std::unique_ptr<Brush>, TypeHash> mBrushes;
    Type mCurrent{Type::Normal};
};
