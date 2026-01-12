#pragma once

#include "render_message.hpp"
#include "../src/brush/brush_manager.hpp"

class BrushChangeMessage : public RenderMessage {
public:
    explicit BrushChangeMessage(BrushManager::Type brushType) : brushType{brushType} {}

    BrushManager::Type brushType;
};
