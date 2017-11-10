#version 430 core

in vec3 outColor;
in vec2 outTexCoord;

out vec4 color;

uniform sampler2D tex;
uniform float currentTime;

void main(){
vec3 colorTemp = outColor * abs(sin(currentTime));
//color = vec4(colorTemp, 1.0f);
color = texture(tex, outTexCoord);
}