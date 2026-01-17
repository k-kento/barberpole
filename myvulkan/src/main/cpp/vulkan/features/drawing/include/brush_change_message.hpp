#pragma once

#include "../src/brush/brush_manager.hpp"
#include "render_message.hpp"

class BrushChangeMessage : public RenderMessage {
   public:
    explicit BrushChangeMessage(BrushManager::Type brushType) : brushType{brushType} {}

    BrushManager::Type brushType;
};
