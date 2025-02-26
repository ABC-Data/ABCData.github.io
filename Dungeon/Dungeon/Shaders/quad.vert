#version 450 core

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;
layout (location=2) in vec2 aVertexTextureCoords;
layout (location=3) in float aTextureIndex;
		
out vec4 vColor;
out vec2 vTextureCoord;	
out float vTextureIndex;
out mat4 vTransform;
out mat4 vViewProj;
out float vDepth;

uniform mat4 uTransform;
uniform mat4 uViewProj;	
void main(void) 
{   
vTextureCoord = aVertexTextureCoords;
vColor = aVertexColor;	
vTextureIndex = aTextureIndex;
vTransform = uTransform;
vViewProj = uViewProj;
vDepth = aVertexPosition.z;
gl_Position = uViewProj * uTransform * vec4(aVertexPosition, 1.0);
}