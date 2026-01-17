#pragma once

#include "glm/glm.hpp"
#include "render_message.hpp"
#include "view_bounds.hpp"

class SurfaceChangedMessage : public RenderMessage {
   public:
    explicit SurfaceChangedMessage(ViewBounds viewBounds, glm::mat4 projection)
        : viewBounds{viewBounds}, projection{projection} {}

    ViewBounds viewBounds;
    glm::mat4 projection;
};
