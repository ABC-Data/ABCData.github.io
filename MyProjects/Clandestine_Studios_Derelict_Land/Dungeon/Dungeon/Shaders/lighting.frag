#version 450 core

uniform vec2 light_pos[32];
uniform vec3 light_color[32];
uniform float light_intensity[32];

layout (location = 0) out vec4 fFragColor;

void main(void)        
{                                             
vec3 ambient = vec3(0.2, 0.2, 0.2); // Ambient light color
vec3 diffuse = vec3(0.0);// Diffuse light color
vec3 position = vec3(gl_FragCoord.xy, 0.0); // Pixel position

for(int i = 0; i < 32; i++) {
	vec3 lightDir = normalize(vec3(light_pos[i], 0.0) - position );
	float diff = max(dot(lightDir, normalize(vec3(0.0, 0.0, -1.0))), 0.0);
	diffuse += diff * light_intensity[i] * light_color[i];
}

fFragColor = vec4((ambient + diffuse), 1.0);
}