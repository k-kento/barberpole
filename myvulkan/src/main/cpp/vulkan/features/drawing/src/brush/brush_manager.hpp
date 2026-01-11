#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "brush.hpp"
#include "normal/normal_brush.hpp"
#include "rainbow/rainbow_brush.hpp"
#include "../pipeline/pipeline_manager.hpp"

class BrushManager {
public:
    enum class Type {
        Normal,
        Rainbow
    };

    BrushManager(VulkanContext& vulkanContext, PipelineManager& pipelineManager) {
        registerBrush(Type::Normal, std::make_unique<NormalBrush>(vulkanContext, pipelineManager.get("normal")));
        registerBrush(Type::Rainbow, std::make_unique<RainbowBrush>(vulkanContext, pipelineManager.get("rainbow")));
        set(Type::Rainbow);
    }

    void set(Type type) {
        if (mBrushes.find(type) == mBrushes.end()) {
            throw std::runtime_error("Brush type not registered.");
        }
        mCurrent = type;
    }

    Brush& current() {
        return *mBrushes.at(mCurrent);
    }

    const Brush& current() const {
        return *mBrushes.at(mCurrent);
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
