#version 330

layout(location = 0) in vec3 posicion_vertice;
layout(loaction = 1) in vec3 color_vertice;
layout(location = 2) in vec3 normal_vertice;

out vec3 color_fragment;

uniform mat4 MVP;


void main()
{
    gl_Position = MVP * vec4(posicion_vertice,1.0);
    
    color_fragment = normal_vertice;
}