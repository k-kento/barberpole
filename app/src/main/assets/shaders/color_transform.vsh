#version 300 es

uniform mat4 u_MvpModel;

in vec4 a_Position;
in vec4 a_Color;
out vec4 v_Color;

void main() {
    gl_Position = u_MvpModel * a_Position;
    v_Color = a_Color;
}
