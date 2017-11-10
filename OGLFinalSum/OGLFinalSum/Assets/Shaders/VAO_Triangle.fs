#version 430 core

in vec3 outColor;
in vec2 outTexCoord;
in vec4 mWorldPos;


out vec4 color;

uniform sampler2D tex;
uniform float currentTime;
uniform bool cocolor;
uniform vec3 CameraPos;


void main(){

if(cocolor == true)
{
   color =  vec4(0.04, 0.28, 0.26, 1.0);
}
else
{
   float d = distance(mWorldPos.xyz, CameraPos);
   float lerp = (d - 5.0f)/10.f;
   lerp = clamp(lerp, 0.0, 1.0);
   vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);

   color = texture(tex, outTexCoord);
   color = mix(color, vFogColor, lerp);
}


}