#version 430 core

in vec2 TexCoord;

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

// texture
uniform sampler2D Texture;
uniform float currentTime;

//lighting
uniform vec3 objectColor;

uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float specularStrength;
uniform float ambientStrength;

void main(){
vec3 ambient = ambientStrength * lightColor;

//**diffuse
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

float intensity = dot(lightDir,norm);

//**specular 
vec3 viewDir = normalize(cameraPos - FragPos);
vec3 reflectionDir = reflect(-lightDir, norm);

vec3 H = normalize( lightDir + viewDir);
vec3 specular = pow(max(dot(norm, H), 0.0), 256) * lightColor * specularStrength;


vec3 totalColor = ambient + diffuse + specular*objectColor; 

	if (intensity > 0.95){
		color = vec4(1.0,0.5,0.5,1.0) *texture(Texture, TexCoord);}
	else if (intensity > 0.5){
		color = vec4(0.6,0.3,0.3,1.0)*texture(Texture, TexCoord);}
	else if (intensity > 0.25){
		color = vec4(0.4,0.2,0.2,1.0)*texture(Texture, TexCoord);}
	else{
		color = vec4(0.2,0.1,0.1,1.0)*texture(Texture, TexCoord);}

}