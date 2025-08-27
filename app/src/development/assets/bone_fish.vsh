#version 300 es
layout(location = 0) in vec4 aPosition;
layout(location = 1) in uvec4 aBoneIndices;  // 整数型
layout(location = 2) in vec4 aBoneWeights;

uniform mat4 uMvpMatrix;
uniform mat4 uBoneMatrices[8]; // 最大ボーン数に合わせる

void main() {
    mat4 skinMatrix =
    aBoneWeights.x * uBoneMatrices[int(aBoneIndices.x)] +
    aBoneWeights.y * uBoneMatrices[int(aBoneIndices.y)] +
    aBoneWeights.z * uBoneMatrices[int(aBoneIndices.z)] +
    aBoneWeights.w * uBoneMatrices[int(aBoneIndices.w)];

    vec4 skinnedPosition = skinMatrix * aPosition;
    gl_Position = uMvpMatrix * skinnedPosition;
}