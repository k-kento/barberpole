#pragma once

#include "glm/glm.hpp"

struct InputVertex {
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 uv;  // x: 中心からの距離 (0~1), y: 未使用
};
