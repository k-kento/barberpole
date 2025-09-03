#version 300 es
layout(location = 0) in vec4 aPosition;

uniform mat4 uMvpMatrix;

void main() {
    gl_Position = uMvpMatrix * aPosition;
}