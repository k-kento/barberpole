#version 300 es

in vec4 a_Position;
uniform mat4 u_MvpMatrix;
out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_Position.xy;
    gl_Position = u_MvpMatrix * a_Position;
}