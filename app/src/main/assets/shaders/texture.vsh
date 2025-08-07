#version 300 es
precision mediump float;

in vec4 a_Position;
in vec2 a_TexCoord;
out vec2 v_TexCoord;
uniform mat4 u_MvpMatrix;

void main() {
    gl_Position = u_MvpMatrix * a_Position;
    v_TexCoord = a_TexCoord;
}