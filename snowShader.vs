#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;

uniform mat4 view;
uniform mat4 projection;
uniform int affine;

void main()
{
    color = aColor; 
    gl_Position = projection * view * model *vec4(aPos.x,aPos.y*affine,aPos.z, 1.0);
}