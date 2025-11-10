#pragma once

#include "stdint.h"

// Compute shader のローカルワークグループサイズ
constexpr uint32_t WORKGROUP_SIZE = 64;

// 入力可能な最大点数
constexpr uint32_t MAX_INPUT_POINTS = 1000; // TODO 超えた場合の対応

// ComputeShader 1線分あたりの出力頂点数
constexpr uint32_t COMPUTE_SHADER_OUTPUT_VERTEX_NUM = 2;

// 出力バッファに必要な最大頂点数
constexpr uint32_t MAX_COMPUTE_OUTPUT_VERTICES = MAX_INPUT_POINTS * COMPUTE_SHADER_OUTPUT_VERTEX_NUM;
