#version 450 core

layout (location=0) in vec3 aVertexPosition;

out vec4 vColor;

uniform mat4 uTransform;
uniform mat4 uViewProj;	
void main(void) 
{
gl_Position = uViewProj * uTransform * vec4(aVertexPosition, 1.0);
}