#pragma once

#include "render_message.hpp"
#include "rotation_state.hpp"

class RotationMessage : public RenderMessage {
   public:
    explicit RotationMessage(RotationState state) : rotationState{state} {}

    RotationState rotationState;
};
