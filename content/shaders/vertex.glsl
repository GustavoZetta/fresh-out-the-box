#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

uniform mat4 model;
uniform mat4 projection;

uniform vec2 uvOffset;
uniform vec2 uvSize;

out vec2 texCoords;

void main() {
    texCoords = uvOffset + vertex.zw * uvSize;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
