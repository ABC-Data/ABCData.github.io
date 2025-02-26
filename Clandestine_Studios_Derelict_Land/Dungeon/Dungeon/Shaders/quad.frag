#version 450 core

in vec4 vColor;
in vec2 vTextureCoord;
in float vTextureIndex;
in mat4 vTransform;
in mat4 vViewProj;
in float vDepth;

uniform sampler2D uTex2d[32];
uniform vec3 light_pos[32];
uniform vec3 light_color[32];
uniform float light_intensity[32];
uniform int light_size;

layout (location = 0) out vec4 fFragColor;

void main(void)        
{   


vec3 ambient = vec3(0.7); // Ambient light color
vec3 diffuse = vec3(0.0);// Diffuse light color
vec3 position = vec3(vTextureCoord, 0.0); // Pixel position


for(int i = 0; i < light_size; i++) {
	if(vDepth > light_pos[i].z)
		break;
	vec3 lightDir = normalize(vec3(vViewProj * (vTransform/2.0) * vec4(light_pos[i].xy, light_pos[i].z, 1.0)) - position);
	float diff = max(dot(lightDir, normalize(vec3(0.0, 0.0, -1.0))), 0.0);
	diffuse += diff * (light_intensity[i]*0.1) * light_color[i];
}
                                          
int index = int(vTextureIndex);
vec4 texclr = texture(uTex2d[index],vTextureCoord) * vColor;
if(texclr.a < 0.1)
	discard;

fFragColor = texclr * vec4((ambient + diffuse), 1.0);
}