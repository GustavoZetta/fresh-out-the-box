#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

uniform int useTexture;

void main() {
    if (useTexture == 0) {
        color = spriteColor;
    } else {
        color = spriteColor * texture(image, texCoords);
    }
}
