#version 450 core

in vec4 vColor;	

layout (location = 0) out vec4 fFragColor;

void main(void)
{
fFragColor = vColor;
}