#pragma once

#include "render_message.hpp"

class TouchMessage : public RenderMessage {

public:

    enum TouchType {
        Move,
        UP
    };

    TouchMessage(TouchType touchType, float x = 0.0f, float y = 0.0f) : touchType{touchType}, x{x}, y{y} {}

    TouchType touchType;
    float x;
    float y;

};
