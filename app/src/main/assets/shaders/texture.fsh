#version 300 es
precision mediump float;

// テクスチャサンプラ
uniform sampler2D u_Texture;

uniform vec3 u_Color;

// フラグメントシェーダーへのUV座標（頂点シェーダーから）
in vec2 v_TexCoord;

// 出力色
out vec4 outColor;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);

    // RGB に u_Color を掛けて色を変える（αはそのまま）
    texColor.rgb *= u_Color;
    outColor = texColor;
}