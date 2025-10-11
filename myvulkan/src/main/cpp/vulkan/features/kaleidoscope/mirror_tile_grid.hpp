#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "view_bounds.hpp"

constexpr float SCALE = 0.25f;

class MirrorTileGrid {

public:

    // タイルを並べる
    static std::vector<glm::mat4> createTileGrid(ViewBounds *viewBounds) {

        float screenWidth = viewBounds->width();
        float screenHeight = viewBounds->height();

        float tileWidth = 2 * sqrt(3) * SCALE;
        float tileHeight = 3.0f * SCALE;

        float dx = (tileWidth + tileWidth / 2.0f);
        float dy = tileHeight / 2;

        int cols = static_cast<int>(screenWidth / dx) + 1;
        int rows = static_cast<int>(screenHeight / dy) + 2;

        LOGI("cols=%d, rows=%d", cols, rows);

        auto tiles = createTile();

        std::vector<glm::mat4> transforms;
        auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(SCALE));

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                float offsetX = (y % 2 == 0) ? 0.0f : dx / 2.0f;
                float px = x * dx + offsetX - screenWidth / 2.0f;
                float py = y * dy - screenHeight / 2.0f;

                glm::mat4 tileTranslation = glm::translate(glm::mat4(1.0f),
                                                           glm::vec3(px, py, 0.0f));
                for (auto &slice: tiles) {
                    glm::mat4 newModel = tileTranslation * scale * slice;
                    transforms.push_back(newModel);
                }
            }
        }

        return transforms;
    }

private:

    struct SliceInfo {
        glm::vec3 offset;
        float rotate;
        bool mirror;
    };

    // タイル作成
    static std::vector<glm::mat4> createTile() {
        std::vector<glm::mat4> transforms;

        // 1辺の半分の長さ
        float halfLength = sin(glm::radians(60.0f));

        SliceInfo slices[] = {
                {{0.0f,        1.0f,  0.0f}, 0.0f,   false},
                {{-halfLength, -0.5f, 0.0f}, 120.0f, false},
                {{halfLength,  -0.5f, 0.0f}, 240.0f, false},
                {{0.0f,        -1.0f, 0.0f}, 0.0f,   true},
                {{halfLength,  0.5f,  0.0f}, 120.0f, true},
                {{-halfLength, 0.5f,  0.0f}, 240.0f, true}
        };

        for (auto &s: slices) {
            glm::mat4 model = createSlice(s.offset, s.rotate, s.mirror);
            transforms.push_back(model);
        }

        return transforms;
    }

    static glm::mat4 createSlice(glm::vec3 translateVec, float rotateDeg, bool mirrorY) {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, translateVec);
        if (rotateDeg != 0.0f)
            transform = glm::rotate(transform, glm::radians(rotateDeg), glm::vec3(0, 0, 1));
        if (mirrorY)
            transform = glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f));
        return transform;
    }
};
