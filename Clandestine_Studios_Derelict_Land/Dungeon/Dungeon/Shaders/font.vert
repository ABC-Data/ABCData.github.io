#version 450 core

uniform mat4 Projection_Mat4;
uniform float float_depth;
                                        
layout (location=0) in vec4 vertex;
out vec2 TexCoords;

void main() 
{ 
//gl_Position = Projection_Mat4 * vec4(vertex.x, vertex.y , 0.999999, 1.0);
gl_Position = Projection_Mat4 * vec4(vertex.x, vertex.y , float_depth , 1.0);
TexCoords = vec2(vertex.z,vertex.w);
}