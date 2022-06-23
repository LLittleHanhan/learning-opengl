#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox0;
uniform samplerCube skybox1;
uniform float m;

void main()
{    
    FragColor = mix(texture(skybox0, TexCoords),texture(skybox1, TexCoords),m);
}