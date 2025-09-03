#version 300 es
precision mediump float;

uniform vec4 uColor; // 単色

out vec4 fragColor;

void main() {
    fragColor = uColor;
}