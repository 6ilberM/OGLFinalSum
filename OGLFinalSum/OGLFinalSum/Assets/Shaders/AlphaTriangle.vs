#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normalDir;

out vec3 outColor;
out vec2 outTexCoord;
out vec3 outNormalDir;

uniform mat4 mvp;
uniform mat4 world;

void main() {
gl_Position = mvp * vec4(position, 1.0);
outColor = color;
outTexCoord = texCoord;
outNormalDir = normalDir;
}