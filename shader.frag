#version 330 core

in vec3 color;
in vec2 v_texture;

out vec4 frag_color;

uniform sampler2D tex0;

void main() {
    frag_color = texture(tex0, v_texture);
}

