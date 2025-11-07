#version 450
layout (points) in;
layout (triangle_strip, max_vertices = 4) out; // 四角を出す

layout (location = 0) in float gsSize[];
layout (location = 1) in vec3 gsColor[];

//layout(location = 0) out vec2 fsUV;
//layout(location = 1) out vec3 fsColor;

void main() {
    // 入力は gl_in[0].gl_Position (クリップ空間)
    float halfSize = gsSize[0] * 0.5;

    // ここでは簡単のためクリップ空間でオフセット（実際は画面ピクセル単位の処理をするならNDC->screenに変換して調整）
    // 中心点
    vec4 p = gl_in[0].gl_Position;

    // 四角を作る（順序に注意）
    vec4 offsets[4] = vec4[](
        vec4(-halfSize, -halfSize, 0.0, 0.0),
        vec4(halfSize, -halfSize, 0.0, 0.0),
        vec4(-halfSize, halfSize, 0.0, 0.0),
        vec4(halfSize, halfSize, 0.0, 0.0));

    for (int i = 0; i < 4; ++i) {
        gl_Position = p + offsets[i];
        //        fsUV = (offsets[i].xy / gsSize[0]) + vec2(0.5); // uv [0,1]
        //        fsColor = gsColor[0];
        EmitVertex();
    }
    EndPrimitive();
}
