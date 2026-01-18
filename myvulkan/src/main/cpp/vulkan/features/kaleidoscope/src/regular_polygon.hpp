#pragma once

#include <vector>

#include "vertex.hpp"
#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"

class RegularPolygon {
   public:
    RegularPolygon(int num, float radius) {
        float unit = 2.0f * glm::pi<float>() / static_cast<float>(num);

        // 中心頂点
        Vertex center{};
        center.position = glm::vec2(0.0f, 0.0f);
        center.uv = glm::vec2(0.5f, 0.5f);
        vertices.push_back(center);

        // 外周の頂点
        for (int i = 0; i < num; i++) {
            float theta = unit * static_cast<float>(i) - glm::pi<float>() / 2.0f;  // 上スタート
            float x = std::cos(theta) * radius;
            float y = std::sin(theta) * radius;

            Vertex vertex{};
            vertex.position = glm::vec2(x, y);
            vertex.uv = glm::vec2((x / (2.0f * radius)) + 0.5f, (y / (2.0f * radius)) + 0.5f);
            vertices.push_back(vertex);
        }

        for (int i = 1; i <= num; i++) {
            indices.push_back(0);                    // 中心
            indices.push_back(i < num ? i + 1 : 1);  // 次の頂点
            indices.push_back(i);                    // 現在の頂点
        }
    }

   public:
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
};
