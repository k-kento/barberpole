#pragma once

#include "glm/glm.hpp"

// binding は例。std430 に注意
struct Bubble {
    glm::vec2 pos;     // 画面座標 [-1,1] or ピクセル座標。どちらかで統一
    glm::vec2 vel;
    float radius; // ピクセル or NDC半径
    float life;   // 0..1
};
