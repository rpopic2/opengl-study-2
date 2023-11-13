#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 color;

uniform float scale;

void main() {
    gl_Position = vec4(pos * scale, 1.0);
    color = col;
}

