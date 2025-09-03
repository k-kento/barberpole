#version 300 es
precision mediump float;

// テクスチャサンプラ
uniform sampler2D u_Texture;

uniform vec4 u_Color;

// フラグメントシェーダーへのUV座標（頂点シェーダーから）
in vec2 v_TexCoord;

// 出力色
out vec4 outColor;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    // テクスチャの明暗を保って色を変える
    texColor.rgb =  u_Color.rgb;
    // アルファも反映
    texColor.a *= u_Color.a;
    outColor = texColor;
}
