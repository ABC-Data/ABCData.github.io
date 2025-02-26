#version 450 core

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;

out vec4 vColor;

uniform mat4 uTransform;
uniform mat4 uViewProj;
void main(void)
{
vColor = aVertexColor;
gl_Position = uViewProj * uTransform * vec4(aVertexPosition, 1.0);
}