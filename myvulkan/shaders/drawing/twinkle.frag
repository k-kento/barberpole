#version 100

layout (std430, binding = 0) readonly buffer StarBuffer;
struct Star {
    vec2 pos;
    float offset;
    float seed;
};

Star stars[];

void main() {

}
