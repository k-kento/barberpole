#pragma once

#include "glm/glm.hpp"

struct UboData {
    glm::mat4 projection;  // 64 bytes
    float time;            // 4 bytes
    float glowIntensity;   // 4 bytes (0.0~1.0)
    float glowRadius;      // 4 bytes (normalized)
    float pulsePeriod;     // 4 bytes (0.5~1.5秒)
    // Total: 80 bytes (16バイトアライメント済み)
};
